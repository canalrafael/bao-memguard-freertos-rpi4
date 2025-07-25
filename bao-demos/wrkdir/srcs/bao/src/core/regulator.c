// Filename: regulation.c
// Author: Everaldo Pereira Gomes

#include <regulator.h>

struct Regulation_config reg_conf[VM_QNT];

void init_regulation_config()
{
    for (uint8_t vm_num = 0; vm_num < VM_QNT; vm_num++) {
        // VW
        reg_conf[vm_num].vm.depleated_op_type = UNKNOWN_VALUE;
        reg_conf[vm_num].vm.current_used_read_budget = 0;
        reg_conf[vm_num].vm.current_used_write_budget = 0;
        reg_conf[vm_num].vm.total_used_read_budget = 0;
        reg_conf[vm_num].vm.total_used_write_budget = 0;
        reg_conf[vm_num].vm.total_calculated_new_read_budget = 0;
        reg_conf[vm_num].vm.total_calculated_new_write_budget = 0;
        reg_conf[vm_num].vm.new_read_budget = 0;
        reg_conf[vm_num].vm.new_write_budget = 0;

        // EWMA
        reg_conf[vm_num].ewma.previous_predicted_read_budget = 0;
        reg_conf[vm_num].ewma.previous_predicted_write_budget = 0;
        reg_conf[vm_num].ewma.new_read_budget = 0;
        reg_conf[vm_num].ewma.new_write_budget = 0;
        reg_conf[vm_num].ewma.alpha = 2;
        reg_conf[vm_num].ewma.scaling_factor = 10;

        // SW
        for (uint8_t i_sw = 0; i_sw < SW_MAX_ARRAY_SIZE; i_sw++) {
            reg_conf[vm_num].sw.read_usage[i_sw] = 0;
            reg_conf[vm_num].sw.write_usage[i_sw] = 0;
        }

        reg_conf[vm_num].sw.current_read_array_size = 0;
        reg_conf[vm_num].sw.current_write_array_size = 0;
        reg_conf[vm_num].sw.read_index = 0;
        reg_conf[vm_num].sw.write_index = 0;

        // AMBP
        reg_conf[vm_num].ambp.budget_read_limit = 100;
        reg_conf[vm_num].ambp.budget_write_limit = 100;
        reg_conf[vm_num].ambp.qnt_budget_read_limit_reached = 0;
        reg_conf[vm_num].ambp.qnt_budget_write_limit_reached = 0;
        reg_conf[vm_num].ambp.penalty_by_reaching_budget_read_limit = 0;
        reg_conf[vm_num].ambp.penalty_by_reaching_budget_write_limit = 0;
        reg_conf[vm_num].ambp.alpha = 2;
        reg_conf[vm_num].ambp.scaling_factor = 10;

        // AFC
        reg_conf[vm_num].afc.previous_read_budget = 0;
        reg_conf[vm_num].afc.previous_write_budget = 0;
        reg_conf[vm_num].afc.proportional_gain = 2;
        reg_conf[vm_num].afc.scaling_factor = 10;

        // LR
        for (uint8_t i_lr = 0; i_lr < LR_MAX_QNT_ACCESS; i_lr++) {
            reg_conf[vm_num].lr.t_vector[i_lr] = i_lr;
            reg_conf[vm_num].lr.read_usage[i_lr] = 0;
            reg_conf[vm_num].lr.write_usage[i_lr] = 0;
            reg_conf[vm_num].lr.total_read_exec = 0;
            reg_conf[vm_num].lr.total_write_exec = 0;
        }

        reg_conf[vm_num].lr.current_read_array_size = 0;
        reg_conf[vm_num].lr.current_write_array_size = 0;

        // PIC
        reg_conf[vm_num].pic.accumulated_read_error = 0;
        reg_conf[vm_num].pic.accumulated_write_error = 0;
        reg_conf[vm_num].pic.kp = 2;
        reg_conf[vm_num].pic.ki = 1;
        reg_conf[vm_num].pic.scaling_factor = 10;
    }
}

inline static uint32_t get_operation_usage(const uint8_t cpu_id,
                                           const uint8_t task_num,
                                           uint8_t op_type)
{
    uint32_t pmu_counter_0 = PMU_get_counter_value(0);
    uint32_t pmu_counter_1 = PMU_get_counter_value(1);
    uint32_t r_reg = (uint32_t)MAX_INT - pmu_counter_0;
    uint32_t w_reg = (uint32_t)MAX_INT - pmu_counter_1;
    return op_type == READ ? r_reg : w_reg;
}

inline static void ewma(const uint8_t cpu_id, const uint8_t task_num_)
{
    // printk("[BAO] ewma, cpu %d, task %d\n", cpu_id, task_num_);
    // PMU_print_all_counters();
    const uint32_t current_read_usage =
        get_operation_usage(cpu_id, UNUSED_ARG, READ);

    if (current_read_usage == 0) {
        printk("[BAO] current_read_usage == 0\n");
    }

    if (current_read_usage != 0 && current_read_usage < MARGIN) {
        // printk("[BAO] OK current_read_usage != 0\n");
        reg_conf[cpu_id].vm.current_used_read_budget = current_read_usage;
        reg_conf[cpu_id].vm.total_used_read_budget += current_read_usage;
        reg_conf[cpu_id].ewma.previous_predicted_read_budget =
            reg_conf[cpu_id].ewma.new_read_budget;

        reg_conf[cpu_id].ewma.new_read_budget =
            (reg_conf[cpu_id].ewma.alpha * current_read_usage +
             ((reg_conf[cpu_id].ewma.scaling_factor -
               reg_conf[cpu_id].ewma.alpha) *
              reg_conf[cpu_id].ewma.previous_predicted_read_budget)) /
            reg_conf[cpu_id].ewma.scaling_factor;

        reg_conf[cpu_id].vm.new_read_budget =
            reg_conf[cpu_id].ewma.new_read_budget;
    }

    // WRITE
    const uint32_t current_write_usage =
        get_operation_usage(cpu_id, UNUSED_ARG, WRITE);

    if (current_write_usage == 0) {
        printk("[BAO] current_write_usage == 0\n");
    }

    if (current_write_usage != 0 && current_write_usage < MARGIN) {
        // printk("[BAO] OK current_write_usage != 0\n");
        reg_conf[cpu_id].vm.current_used_write_budget = current_write_usage;
        reg_conf[cpu_id].vm.total_used_write_budget += current_write_usage;
        reg_conf[cpu_id].ewma.previous_predicted_write_budget =
            reg_conf[cpu_id].ewma.new_write_budget;

        reg_conf[cpu_id].ewma.new_write_budget =
            (reg_conf[cpu_id].ewma.alpha * current_write_usage +
             ((reg_conf[cpu_id].ewma.scaling_factor -
               reg_conf[cpu_id].ewma.alpha) *
              reg_conf[cpu_id].ewma.previous_predicted_write_budget)) /
            reg_conf[cpu_id].ewma.scaling_factor;

        reg_conf[cpu_id].vm.new_write_budget =
            reg_conf[cpu_id].ewma.new_write_budget;
    }
}

inline static void sw(const uint8_t cpu_id, const uint8_t task_num_)
{
    // READ
    const uint32_t current_read_usage =
        get_operation_usage(cpu_id, UNUSED_ARG, READ);

    if (current_read_usage != 0 && current_read_usage < MARGIN) {
        // Insert the new usage into the circular buffer
        reg_conf[cpu_id].sw.read_usage[reg_conf[cpu_id].sw.read_index] =
            current_read_usage;
        reg_conf[cpu_id].sw.read_index =
            (reg_conf[cpu_id].sw.read_index + 1) % SW_MAX_ARRAY_SIZE;

        // Update the sample count (up to buffer size)
        if (reg_conf[cpu_id].sw.current_read_array_size < SW_MAX_ARRAY_SIZE)
            reg_conf[cpu_id].sw.current_read_array_size++;

        reg_conf[cpu_id].vm.current_used_read_budget = current_read_usage;

        // Only compute if enough samples exist
        if (reg_conf[cpu_id].sw.current_read_array_size >= SW_MAX_WINDOW_SIZE) {
            uint32_t lowest_avg = MAX_INT;

            // Total number of windows available
            uint8_t num_windows = reg_conf[cpu_id].sw.current_read_array_size -
                                  SW_MAX_WINDOW_SIZE + 1;

            for (uint8_t i = 0; i < num_windows; i++) {
                uint32_t sum = 0;

                for (uint8_t j = 0; j < SW_MAX_WINDOW_SIZE; j++) {
                    uint8_t index =
                        (reg_conf[cpu_id].sw.read_index + SW_MAX_ARRAY_SIZE +
                         i + j - reg_conf[cpu_id].sw.current_read_array_size) %
                        SW_MAX_ARRAY_SIZE;

                    sum += reg_conf[cpu_id].sw.read_usage[index];
                }

                const uint32_t avg =
                    (sum + (SW_MAX_WINDOW_SIZE / 2)) / SW_MAX_WINDOW_SIZE;
                reg_conf[cpu_id].vm.total_used_read_budget += avg;

                if (avg > 0 && avg < lowest_avg) lowest_avg = avg;
            }

            reg_conf[cpu_id].vm.new_read_budget = lowest_avg;
        }
    }

    /* const uint32_t current_read_usage = get_operation_usage(cpu_id, task_num,
     * READ); */

    /* if (current_read_usage != 0 && current_read_usage < MARGIN) */
    /* { */
    /* 	// Insert the new usage into the circular buffer */
    /* 	reg_conf[cpu_id].sw.read_usage[reg_conf[cpu_id].sw.read_index]
     * = current_read_usage; */
    /* 	reg_conf[cpu_id].sw.read_index =
     * (reg_conf[cpu_id].sw.read_index + 1) % SW_MAX_ARRAY_SIZE; */

    /* 	// Track the number of valid samples (max = SW_MAX_ARRAY_SIZE) */
    /* 	if (reg_conf[cpu_id].sw.current_read_array_size <
     * SW_MAX_ARRAY_SIZE) */
    /* 		reg_conf[cpu_id].sw.current_read_array_size++; */

    /* 	reg_conf[cpu_id].vm.current_used_read_budget =
     * current_read_usage; */

    /* 	// If buffer is full, compute sliding window averages */
    /* 	if (reg_conf[cpu_id].sw.current_read_array_size ==
     * SW_MAX_ARRAY_SIZE) */
    /* 	{ */
    /* 		uint32_t lowest_avg = MAX_INT; */

    /* 		// Loop through all possible windows */
    /* 		for (uint8_t i = 0; i <= (SW_MAX_ARRAY_SIZE -
     * SW_MAX_WINDOW_SIZE); i++) */
    /* 		{ */
    /* 			uint32_t sum = 0; */

    /* 			for (uint8_t j = 0; j < SW_MAX_WINDOW_SIZE; j++) */
    /* 			{ */
    /* 				// Circular buffer access */
    /* 				uint8_t index =
     * (reg_conf[cpu_id].sw.read_index + i + j) % SW_MAX_ARRAY_SIZE;
     */
    /* 				sum +=
     * reg_conf[cpu_id].sw.read_usage[index]; */
    /* 			} */

    /* 			const uint32_t avg = (sum + (SW_MAX_WINDOW_SIZE / 2)) /
     * SW_MAX_WINDOW_SIZE; */
    /* 			reg_conf[cpu_id].vm.total_used_read_budget +=
     * avg; */

    /* 			if (avg < lowest_avg && avg > 0) */
    /* 				lowest_avg = avg; */
    /* 		} */

    /* 		reg_conf[cpu_id].vm.new_read_budget = lowest_avg; */
    /* 	} */
    /* } */

    // ORIGINAL
    /* const uint8_t current_read_array_size =
     * reg_conf[cpu_id].sw.current_read_array_size; */
    /* const uint32_t current_read_usage = get_operation_usage(cpu_id, task_num,
     * READ); */

    /* // Fill the array of size SW_MAX_ARRAY_SIZE */
    /* if (current_read_array_size < SW_MAX_ARRAY_SIZE) */
    /* { */
    /* 	if (current_read_usage != 0 && current_read_usage < MARGIN) */
    /* 	{	 */
    /* 		reg_conf[cpu_id].sw.read_usage[current_read_array_size]
     * = current_read_usage; */
    /* 		reg_conf[cpu_id].vm.current_used_read_budget =
     * current_read_usage; */
    /* 		reg_conf[cpu_id].sw.current_read_array_size++; */
    /* 	} */
    /* } */
    /* else */
    /* { */
    /* 	// Sliding the window and calculating the average */
    /* 	uint32_t lowest_avg = MAX_INT; */

    /* 	for (uint8_t array = 0; array <= (SW_MAX_ARRAY_SIZE -
     * SW_MAX_WINDOW_SIZE); array++) */
    /* 	{ */
    /* 		uint32_t sum = 0; */

    /* 		for (uint8_t window = array; window < (array +
     * SW_MAX_WINDOW_SIZE); window++) */
    /* 			sum += reg_conf[cpu_id].sw.read_usage[window];
     */

    /* 		const uint32_t avg = (sum + (SW_MAX_WINDOW_SIZE / 2)) /
     * SW_MAX_WINDOW_SIZE; */
    /* 		reg_conf[cpu_id].vm.total_used_read_budget += avg; */

    /* 		if (avg > 0 && avg < lowest_avg) */
    /* 			lowest_avg = avg; */
    /* 	} */

    /* 	reg_conf[cpu_id].vm.new_read_budget = lowest_avg; */
    /* 	reg_conf[cpu_id].sw.current_read_array_size = 0; */
    /* } */

    // WRITE
    const uint32_t current_write_usage =
        get_operation_usage(cpu_id, UNUSED_ARG, WRITE);

    if (current_write_usage != 0 && current_write_usage < MARGIN) {
        // Insert the new usage into the circular buffer
        reg_conf[cpu_id].sw.write_usage[reg_conf[cpu_id].sw.write_index] =
            current_write_usage;
        reg_conf[cpu_id].sw.write_index =
            (reg_conf[cpu_id].sw.write_index + 1) % SW_MAX_ARRAY_SIZE;

        // Update the sample count (up to buffer size)
        if (reg_conf[cpu_id].sw.current_write_array_size < SW_MAX_ARRAY_SIZE)
            reg_conf[cpu_id].sw.current_write_array_size++;

        reg_conf[cpu_id].vm.current_used_write_budget = current_write_usage;

        // Only compute if enough samples exist
        if (reg_conf[cpu_id].sw.current_write_array_size >=
            SW_MAX_WINDOW_SIZE) {
            uint32_t lowest_avg = MAX_INT;

            // Total number of windows available
            uint8_t num_windows = reg_conf[cpu_id].sw.current_write_array_size -
                                  SW_MAX_WINDOW_SIZE + 1;

            for (uint8_t i = 0; i < num_windows; i++) {
                uint32_t sum = 0;

                for (uint8_t j = 0; j < SW_MAX_WINDOW_SIZE; j++) {
                    uint8_t index =
                        (reg_conf[cpu_id].sw.write_index + SW_MAX_ARRAY_SIZE +
                         i + j - reg_conf[cpu_id].sw.current_write_array_size) %
                        SW_MAX_ARRAY_SIZE;

                    sum += reg_conf[cpu_id].sw.write_usage[index];
                }

                const uint32_t avg =
                    (sum + (SW_MAX_WINDOW_SIZE / 2)) / SW_MAX_WINDOW_SIZE;
                reg_conf[cpu_id].vm.total_used_write_budget += avg;

                if (avg > 0 && avg < lowest_avg) lowest_avg = avg;
            }

            reg_conf[cpu_id].vm.new_write_budget = lowest_avg;
        }
    }
    /* const uint32_t current_write_usage = get_operation_usage(cpu_id,
     * task_num, WRITE); */

    /* if (current_write_usage != 0 && current_write_usage < MARGIN) */
    /* { */
    /* 	// Insert the new usage into the circular buffer */
    /* 	reg_conf[cpu_id].sw.write_usage[reg_conf[cpu_id].sw.write_index]
     * = current_write_usage; */
    /* 	reg_conf[cpu_id].sw.write_index =
     * (reg_conf[cpu_id].sw.write_index + 1) % SW_MAX_ARRAY_SIZE; */

    /* 	// Track the number of valid samples (max = SW_MAX_ARRAY_SIZE) */
    /* 	if (reg_conf[cpu_id].sw.current_write_array_size <
     * SW_MAX_ARRAY_SIZE) */
    /* 		reg_conf[cpu_id].sw.current_write_array_size++; */

    /* 	reg_conf[cpu_id].vm.current_used_write_budget =
     * current_write_usage; */

    /* 	// If buffer is full, compute sliding window averages */
    /* 	if (reg_conf[cpu_id].sw.current_write_array_size ==
     * SW_MAX_ARRAY_SIZE) */
    /* 	{ */
    /* 		uint32_t lowest_avg = MAX_INT; */

    /* 		// Loop through all possible windows */
    /* 		for (uint8_t i = 0; i <= (SW_MAX_ARRAY_SIZE -
     * SW_MAX_WINDOW_SIZE); i++) */
    /* 		{ */
    /* 			uint32_t sum = 0; */

    /* 			for (uint8_t j = 0; j < SW_MAX_WINDOW_SIZE; j++) */
    /* 			{ */
    /* 				// Circular buffer access */
    /* 				uint8_t index =
     * (reg_conf[cpu_id].sw.write_index + i + j) % SW_MAX_ARRAY_SIZE;
     */
    /* 				sum +=
     * reg_conf[cpu_id].sw.write_usage[index]; */
    /* 			} */

    /* 			const uint32_t avg = (sum + (SW_MAX_WINDOW_SIZE / 2)) /
     * SW_MAX_WINDOW_SIZE; */
    /* 			reg_conf[cpu_id].vm.total_used_write_budget +=
     * avg; */

    /* 			if (avg < lowest_avg && avg > 0) */
    /* 				lowest_avg = avg; */
    /* 		} */

    /* 		reg_conf[cpu_id].vm.new_write_budget = lowest_avg; */
    /* 	} */
    /* } */

    // ORIGINAL
    /* const uint8_t current_write_array_size =
     * reg_conf[cpu_id].sw.current_write_array_size; */
    /* const uint32_t current_write_usage = get_operation_usage(cpu_id,
     * task_num, WRITE); */

    /* // Fill the array of size SW_MAX_ARRAY_SIZE */
    /* if (current_write_array_size < SW_MAX_ARRAY_SIZE) */
    /* { */
    /* 	if (current_write_usage != 0 && current_write_usage < MARGIN) */
    /* 	{	 */
    /* 		reg_conf[cpu_id].sw.write_usage[current_write_array_size]
     * = current_write_usage; */
    /* 		reg_conf[cpu_id].vm.current_used_write_budget =
     * current_write_usage; */
    /* 		reg_conf[cpu_id].sw.current_write_array_size++; */
    /* 	} */
    /* } */
    /* else */
    /* { */
    /* 	// Sliding the window and calculating the average */
    /* 	uint32_t lowest_avg = MAX_INT; */

    /* 	for (uint8_t array = 0; array <= (SW_MAX_ARRAY_SIZE -
     * SW_MAX_WINDOW_SIZE); array++) */
    /* 	{ */
    /* 		uint32_t sum = 0; */

    /* 		for (uint8_t window = array; window < (array +
     * SW_MAX_WINDOW_SIZE); window++) */
    /* 			sum +=
     * reg_conf[cpu_id].sw.write_usage[window]; */

    /* 		const uint32_t avg = (sum + (SW_MAX_WINDOW_SIZE / 2)) /
     * SW_MAX_WINDOW_SIZE; */
    /* 		reg_conf[cpu_id].vm.total_used_write_budget += avg; */

    /* 		if (avg > 0 && avg < lowest_avg) */
    /* 			lowest_avg = avg; */
    /* 	} */

    /* 	reg_conf[cpu_id].vm.new_write_budget = lowest_avg; */
    /* 	reg_conf[cpu_id].sw.current_write_array_size = 0; */
    /* } */
}

inline static void ambp(const uint8_t cpu_id, const uint8_t task_num)
{
    // READ
    const uint32_t current_read_usage =
        get_operation_usage(cpu_id, task_num, READ);
    if (current_read_usage != 0 && current_read_usage < MARGIN) {
        reg_conf[cpu_id].vm.total_used_read_budget += current_read_usage;
        reg_conf[cpu_id].vm.current_used_read_budget = current_read_usage;

        if (current_read_usage > reg_conf[cpu_id].ambp.budget_read_limit) {
            reg_conf[cpu_id].ambp.qnt_budget_read_limit_reached++;

            // Update penalty
            reg_conf[cpu_id].ambp.penalty_by_reaching_budget_read_limit *=
                reg_conf[cpu_id].ambp.qnt_budget_read_limit_reached;

            // Calculate the new budget_read_limit using integer arithmetic
            reg_conf[cpu_id].ambp.budget_read_limit =
                reg_conf[cpu_id].ambp.budget_read_limit +
                (reg_conf[cpu_id].ambp.alpha *
                 (current_read_usage -
                  reg_conf[cpu_id].ambp.budget_read_limit)) /
                    reg_conf[cpu_id].ambp.scaling_factor -
                reg_conf[cpu_id].ambp.penalty_by_reaching_budget_read_limit;

            reg_conf[cpu_id].vm.new_read_budget =
                reg_conf[cpu_id].ambp.budget_read_limit;
        } else {
            // Decrease the budget
            reg_conf[cpu_id].ambp.qnt_budget_read_limit_reached = 0;
            reg_conf[cpu_id].vm.new_write_budget =
                (950 * reg_conf[cpu_id].ambp.budget_read_limit +
                 50 * current_read_usage) /
                reg_conf[cpu_id].ambp.scaling_factor;
        }
    }

    // WRITE
    const uint32_t current_write_usage =
        get_operation_usage(cpu_id, task_num, WRITE);
    if (current_write_usage != 0 && current_write_usage < MARGIN) {
        reg_conf[cpu_id].vm.total_used_write_budget += current_write_usage;
        reg_conf[cpu_id].vm.current_used_write_budget = current_write_usage;
        if (current_write_usage > reg_conf[cpu_id].ambp.budget_write_limit) {
            reg_conf[cpu_id].ambp.qnt_budget_write_limit_reached++;

            // Update penalty
            reg_conf[cpu_id].ambp.penalty_by_reaching_budget_write_limit *=
                reg_conf[cpu_id].ambp.qnt_budget_write_limit_reached;

            // Calculate the new budget_write_limit using integer arithmetic
            reg_conf[cpu_id].ambp.budget_write_limit =
                reg_conf[cpu_id].ambp.budget_write_limit +
                (reg_conf[cpu_id].ambp.alpha *
                 (current_write_usage -
                  reg_conf[cpu_id].ambp.budget_write_limit)) /
                    reg_conf[cpu_id].ambp.scaling_factor -
                reg_conf[cpu_id].ambp.penalty_by_reaching_budget_write_limit;

            reg_conf[cpu_id].vm.new_write_budget =
                reg_conf[cpu_id].ambp.budget_write_limit;
        } else {
            // Decrease the budget
            reg_conf[cpu_id].ambp.qnt_budget_write_limit_reached = 0;
            reg_conf[cpu_id].vm.new_write_budget =
                (950 * reg_conf[cpu_id].ambp.budget_write_limit +
                 50 * current_write_usage) /
                reg_conf[cpu_id].ambp.scaling_factor;
        }
    }
}

inline static void afc(const uint8_t cpu_id, const uint8_t task_num)
{
    // READ
    const uint32_t current_read_usage =
        get_operation_usage(cpu_id, task_num, READ);
    if (current_read_usage != 0 && current_read_usage < MARGIN) {
        reg_conf[cpu_id].vm.total_used_read_budget += current_read_usage;
        reg_conf[cpu_id].vm.current_used_read_budget = current_read_usage;
        const uint32_t read_error =
            current_read_usage - reg_conf[cpu_id].afc.previous_read_budget;

        const uint32_t new_read_budget =
            reg_conf[cpu_id].afc.previous_read_budget +
            ((reg_conf[cpu_id].afc.proportional_gain * read_error)) /
                reg_conf[cpu_id].afc.scaling_factor;

        reg_conf[cpu_id].afc.previous_read_budget = new_read_budget;
        reg_conf[cpu_id].vm.new_read_budget = new_read_budget;
    }

    // WRITE
    const uint32_t current_write_usage =
        get_operation_usage(cpu_id, task_num, WRITE);
    if (current_write_usage != 0 && current_write_usage < MARGIN) {
        reg_conf[cpu_id].vm.total_used_write_budget += current_write_usage;
        reg_conf[cpu_id].vm.current_used_write_budget = current_write_usage;

        const uint32_t write_error =
            current_write_usage - reg_conf[cpu_id].afc.previous_write_budget;

        const uint32_t new_write_budget =
            reg_conf[cpu_id].afc.previous_write_budget +
            ((reg_conf[cpu_id].afc.proportional_gain * write_error)) /
                reg_conf[cpu_id].afc.scaling_factor;

        reg_conf[cpu_id].afc.previous_write_budget = new_write_budget;
        reg_conf[cpu_id].vm.new_write_budget = new_write_budget;
    }
}

inline static void lr(const uint8_t cpu_id, const uint8_t task_num)
{
    uint32_t r = 0, w = 0;

    // READ
    const uint32_t current_read_usage =
        get_operation_usage(cpu_id, task_num, READ);

    if (current_read_usage != 0 && current_read_usage < MARGIN) {
        const uint8_t current_read_array_size =
            reg_conf[cpu_id].lr.current_read_array_size;
        if (current_read_array_size < LR_MAX_QNT_ACCESS) {
            reg_conf[cpu_id].lr.read_usage[current_read_array_size] =
                current_read_usage;
            reg_conf[cpu_id].lr.current_read_array_size++;
            reg_conf[cpu_id].vm.current_used_read_budget = current_read_usage;
        }
    }

    if (r && w) {
        for (uint8_t i = 0; i < LR_MAX_QNT_ACCESS; i++)
            reg_conf[cpu_id].lr.t_vector[i] += 1;
    }
}

inline static void pic(const uint8_t cpu_id, const uint8_t task_num)
{
    // READ
    const uint32_t current_read_usage =
        get_operation_usage(cpu_id, task_num, READ);
    if (current_read_usage != 0 && current_read_usage < MARGIN) {
        const uint32_t read_error = MAX_INT - current_read_usage;
        reg_conf[cpu_id].pic.accumulated_read_error += read_error;
        reg_conf[cpu_id].vm.current_used_read_budget = current_read_usage;
        reg_conf[cpu_id].vm.total_used_read_budget += current_read_usage;

        uint32_t read_adjust = ((reg_conf[cpu_id].pic.kp * read_error) +
                                (reg_conf[cpu_id].pic.ki *
                                 reg_conf[cpu_id].pic.accumulated_read_error)) /
                               reg_conf[cpu_id].pic.scaling_factor;

        reg_conf[cpu_id].vm.new_read_budget = read_error + read_adjust;
    }

    // WRITE
    const uint32_t current_write_usage =
        get_operation_usage(cpu_id, task_num, WRITE);
    if (current_write_usage != 0 && current_write_usage < MARGIN) {
        uint32_t write_error = MAX_INT - current_write_usage;
        reg_conf[cpu_id].pic.accumulated_write_error += write_error;
        reg_conf[cpu_id].vm.current_used_write_budget = current_write_usage;
        reg_conf[cpu_id].vm.total_used_write_budget += current_write_usage;

        uint32_t write_adjust =
            ((reg_conf[cpu_id].pic.kp * write_error) +
             (reg_conf[cpu_id].pic.ki *
              reg_conf[cpu_id].pic.accumulated_write_error)) /
            reg_conf[cpu_id].pic.scaling_factor;

        reg_conf[cpu_id].vm.new_write_budget = write_error + write_adjust;
    }
}

void regulator_budget_depleted(const uint8_t task_num, formula_t formula)
{
    switch (formula) {
        case EWMA_FORMULA:
            ewma(cpu()->id, task_num);
            break;
        case SW_FORMULA:
            sw(cpu()->id, task_num);
            break;
        case AMBP_FORMULA:
            ambp(cpu()->id, task_num);
            break;
        case AFC_FORMULA:
            afc(cpu()->id, task_num);
            break;
        case LR_FORMULA:
            lr(cpu()->id, task_num);
            break;
        case PIC_FORMULA:
            pic(cpu()->id, task_num);
            break;
        default:
            printk("something has gone very wrong!\n");
            break;
    }

    reg_conf[cpu()->id].vm.depleated_op_type = UNKNOWN_VALUE;

    // if (task_num == 0)
    PMU_reset_counter(0);
    PMU_reset_counter(1);
    // else
    //     PMU_reset_counter(2);

#if DEBUG
    printk("VM %u task %u, new R: %u, new W: %u, formula: %s\n", cpu()->id,
           task_num, reg_conf[cpu()->id].vm.new_read_budget,
           reg_conf[cpu()->id].vm.new_write_budget, "UNDEFINED");
    // get_current_formula_name());
#endif
}

uint32_t regulator_get_new_budget(const uint8_t task_num, const uint8_t op_type)
{
    return op_type == READ ? reg_conf[cpu()->id].vm.new_read_budget
                           : reg_conf[cpu()->id].vm.new_write_budget;
}

uint32_t regulator_get_current_used_budget(const uint8_t task_num,
                                           const uint8_t op_type)
{
    return op_type == READ ? reg_conf[cpu()->id].vm.current_used_read_budget
                           : reg_conf[cpu()->id].vm.current_used_write_budget;
}

uint32_t regulator_get_total_used_budget(const uint8_t task_num,
                                         const uint8_t op_type)
{
    return op_type == READ ? reg_conf[cpu()->id].vm.total_used_read_budget
                           : reg_conf[cpu()->id].vm.total_used_write_budget;
}
