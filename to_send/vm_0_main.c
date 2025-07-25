#define VM_NUM 3
#define VM_0_REGULATION 1

#if 0
#define PRINT(fmt, ...) printf("[DEBUG] " fmt, ##__VA_ARGS__)
#else
#define PRINT(fmt, ...) ((void)0)
#endif

TaskHandle_t task_handlers[TASK_QUANTITY];

#if VM_0_REGULATION
static void suspend_task_budget_sgi() {
  vm_conf[VM_NUM].sgi_suspend_task_budget = 1;
}

void config_counter() {
  HC_PMU_config_counter(PMU_COUNTER_PAIR_RW, vm_conf[VM_NUM].new_read_budget,
                        vm_conf[VM_NUM].new_write_budget, UNUSED_ARG,
                        UNUSED_ARG);
}

void start_counter() { HC_PMU_start_counter(vm_conf->pmu_counter_pair_rw); }
void stop_counter() { HC_PMU_stop_counter(vm_conf->pmu_counter_pair_rw); }

void stress_task(void *pvParameters) {
  BenchInfo *info = (BenchInfo *)pvParameters;

  while (true) {
    info->function.pointer(); // full core usage
  }
}

#if VM_0_REGULATION
void ctrl_task(void *pvParameters) {
  // task periodicity
  const TickType_t frequency = pdMS_TO_TICKS(PERIOD_MS_TASK_CTRL);
  TickType_t last_wake_time = xTaskGetTickCount();

  // check VM tasks' period
  const TickType_t period_task_any = pdMS_TO_TICKS(PERIOD_MS_TASK_ANY);
  TickType_t last_check_time_task_any = xTaskGetTickCount();

  uint8_t get_budget = 0;
  uint8_t info_showed = 0;
  uint8_t idx = 0;

  while (1) {
    TickType_t current_time_task_any = xTaskGetTickCount();
    if ((current_time_task_any - last_check_time_task_any) >= period_task_any &&
        !get_budget) {
      get_budget = 1;
      last_check_time_task_any = current_time_task_any;
    } else if (vm_conf[VM_NUM].sgi_suspend_task_budget && !get_budget) {
      get_budget = 1;
    }

    if (get_budget) {
      // suspend all tasks
      for (int task_num = 0; task_num < TASK_QUANTITY; ++task_num) {
        // PRINT("suspending task %d\n", task_num);
        vTaskSuspend(task_handlers[task_num]);
      }

      PRINT("calling HC_regulator_get_new_budget\n");
      HC_regulator_budget_depleted(UNUSED_ARG, get_budget_formula());
      if (vm_conf[VM_NUM].new_read_budget + vm_conf[VM_NUM].new_write_budget ==
          0) {
        PRINT("invalid HC_Regulator_budget_depleted call, returning\n");
        break;
      }
      config_counter();
      PRINT("done\n");

      vm_conf[VM_NUM].used_r_budget_period[idx] =
          HC_regulator_get_current_used_budget(UNUSED_ARG, READ);
      vm_conf[VM_NUM].used_w_budget_period[idx] =
          HC_regulator_get_current_used_budget(UNUSED_ARG, WRITE);

      vm_conf[VM_NUM].new_read_budget =
          HC_regulator_get_new_budget(UNUSED_ARG, READ);
      vm_conf[VM_NUM].new_write_budget =
          HC_regulator_get_new_budget(UNUSED_ARG, WRITE);

      vm_conf[VM_NUM].calc_r_budget_period[idx] =
          vm_conf[VM_NUM].new_read_budget;
      vm_conf[VM_NUM].calc_w_budget_period[idx] =
          vm_conf[VM_NUM].new_write_budget;

      if (idx < PERIOD_QNT && vm_conf[VM_NUM].new_read_budget != 0 &&
          vm_conf[VM_NUM].new_write_budget != 0) {

        idx++;
      }

      get_budget = 0;
      vm_conf[VM_NUM].sgi_suspend_task_budget = 0;

      // resume all tasks
      for (int task_num = 0; task_num < TASK_QUANTITY; ++task_num) {
        // printf("resuming task %d\n", task_num);
        vTaskResume(task_handlers[task_num]);
      }
    }

    // showing results
    if (idx >= 10 && /* task_conf.show_exe_info && */ !info_showed) {
      // vTaskDelay((3500));

      // printf("showing results\n");
      print_vm_info(vm_conf[VM_NUM]);
      idx = 0;

      formula_t formula = get_budget_formula() + 1;
      if (formula >= FORMULA_COUNT) {
        info_showed = 1;
      } else {
        // task_conf.show_exe_info = 0;
        set_budget_formula(formula);
      }
    }

    vTaskDelayUntil(&last_wake_time, frequency);
    last_wake_time = xTaskGetTickCount();
  }

  vTaskDelete(NULL);
}
#endif

void delayed_task(void *pvParameters) {
  BenchInfo *info = (BenchInfo *)pvParameters;

  const TickType_t period = pdMS_TO_TICKS(PERIOD_MS_TASK_CTRL);
  TickType_t last_wake_time = xTaskGetTickCount();

  while (true) {
    info->function.pointer();

    TickType_t now = xTaskGetTickCount();
    if ((now - last_wake_time) > period) {
      info->task_overruns += 1;
      last_wake_time = now;
    } else {
      info->task_underruns += 1;
    }

    vTaskDelayUntil(&last_wake_time, period);
  }
}

int main(void) {
  init_bench();
  print_vm_header();

#if VM_0_REGULATION
  irq_set_handler(GUEST_SUSPEND_BUDGET_ID, suspend_task_budget_sgi);
  irq_enable(GUEST_SUSPEND_BUDGET_ID);
  irq_set_prio(GUEST_SUSPEND_BUDGET_ID, 0);

  config_counter();
  xTaskCreate(ctrl_task, "vm_ctrl_task", 1400, NULL, CTRL_TASK_PRIORITY, NULL);

  start_counter();
#endif

  for (int task_num = 0; task_num < TASK_QUANTITY; ++task_num) {
    BenchInfo *info = add_benchmark_info(VM_NUM, task_num, NULL);
    TaskHandle_t handler;
    xTaskCreate(
        delayed_task,            //
        info->function.name,     //
        TASK_STACK_SIZE,         //
        info,                    // pvParameters to delayed_task
        OTHER_TASK_PRIORITY,     // priority
        &task_handlers[task_num] // where to store the retuned TaskHandler_t
    );
  }

#ifdef VM_0_REGULATION
  stop_counter();
#endif

  vTaskStartScheduler();
  while (true) {
    //
  }
  destroy_bench();
  return 0;
}
