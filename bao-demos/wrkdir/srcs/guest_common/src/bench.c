#include "../inc/bench.h"
#include "../inc/bench_functions.h"
#include "../inc/regulation.h"

#include <stdbool.h>

// Global variables for all benchmark functions
#define NUM_BENCHMARKS 960
#define MAX_TASKS 8

bool init = false;

Function benchmark_functions[NUM_BENCHMARKS] = {
    // ===========================================================================
    // ==   Category A: Progressive Saturation Scenarios (Scenarios 1-40) ==
    // ===========================================================================

    // --- Sub-Category: 1 Active VM (Scenarios 1-8) ---
    // Scenario 1: vm0(H,H), others idle
    {0, bandwidth_wrapper, "bandwidth_wrapper"},
    {1, disparity_wrapper, "disparity_wrapper"},
    {2, empty_wrapper, "empty"},
    {3, empty_wrapper, "empty"},
    {4, empty_wrapper, "empty"},
    {5, empty_wrapper, "empty"},
    {6, empty_wrapper, "empty"},
    {7, empty_wrapper, "empty"},
    // Scenario 2: vm0(H,H), others idle
    {8, fft_wrapper, "fft_wrapper"},
    {9, mser_wrapper, "mser_wrapper"},
    {10, empty_wrapper, "empty"},
    {11, empty_wrapper, "empty"},
    {12, empty_wrapper, "empty"},
    {13, empty_wrapper, "empty"},
    {14, empty_wrapper, "empty"},
    {15, empty_wrapper, "empty"},
    // Scenario 3: vm0(L,L), others idle
    {16, sorting_wrapper, "sorting_wrapper"},
    {17, qsort_wrapper, "qsort_wrapper"},
    {18, empty_wrapper, "empty"},
    {19, empty_wrapper, "empty"},
    {20, empty_wrapper, "empty"},
    {21, empty_wrapper, "empty"},
    {22, empty_wrapper, "empty"},
    {23, empty_wrapper, "empty"},
    // Scenario 4: vm0(L,L), others idle
    {24, sha_wrapper, "sha_wrapper"},
    {25, dijkstra_wrapper, "dijkstra_wrapper"},
    {26, empty_wrapper, "empty"},
    {27, empty_wrapper, "empty"},
    {28, empty_wrapper, "empty"},
    {29, empty_wrapper, "empty"},
    {30, empty_wrapper, "empty"},
    {31, empty_wrapper, "empty"},
    // Scenario 5: vm0(H,L), others idle
    {32, bandwidth_wrapper, "bandwidth_wrapper"},
    {33, qsort_wrapper, "qsort_wrapper"},
    {34, empty_wrapper, "empty"},
    {35, empty_wrapper, "empty"},
    {36, empty_wrapper, "empty"},
    {37, empty_wrapper, "empty"},
    {38, empty_wrapper, "empty"},
    {39, empty_wrapper, "empty"},
    // Scenario 6: vm0(H,L), others idle
    {40, fft_wrapper, "fft_wrapper"},
    {41, sha_wrapper, "sha_wrapper"},
    {42, empty_wrapper, "empty"},
    {43, empty_wrapper, "empty"},
    {44, empty_wrapper, "empty"},
    {45, empty_wrapper, "empty"},
    {46, empty_wrapper, "empty"},
    {47, empty_wrapper, "empty"},
    // Scenario 7: vm0(L,H), others idle
    {48, sorting_wrapper, "sorting_wrapper"},
    {49, disparity_wrapper, "disparity_wrapper"},
    {50, empty_wrapper, "empty"},
    {51, empty_wrapper, "empty"},
    {52, empty_wrapper, "empty"},
    {53, empty_wrapper, "empty"},
    {54, empty_wrapper, "empty"},
    {55, empty_wrapper, "empty"},
    // Scenario 8: vm0(L,H), others idle
    {56, dijkstra_wrapper, "dijkstra_wrapper"},
    {57, mser_wrapper, "mser_wrapper"},
    {58, empty_wrapper, "empty"},
    {59, empty_wrapper, "empty"},
    {60, empty_wrapper, "empty"},
    {61, empty_wrapper, "empty"},
    {62, empty_wrapper, "empty"},
    {63, empty_wrapper, "empty"},

    // --- Sub-Category: 2 Active VMs (Scenarios 9-20) ---
    // Scenario 9: vm0(H,H), vm1(H,H)
    {64, bandwidth_wrapper, "bandwidth_wrapper"},
    {65, disparity_wrapper, "disparity_wrapper"},
    {66, fft_wrapper, "fft_wrapper"},
    {67, mser_wrapper, "mser_wrapper"},
    {68, empty_wrapper, "empty"},
    {69, empty_wrapper, "empty"},
    {70, empty_wrapper, "empty"},
    {71, empty_wrapper, "empty"},
    // Scenario 10: vm0(H,H), vm1(L,L)
    {72, bandwidth_wrapper, "bandwidth_wrapper"},
    {73, disparity_wrapper, "disparity_wrapper"},
    {74, qsort_wrapper, "qsort_wrapper"},
    {75, sorting_wrapper, "sorting_wrapper"},
    {76, empty_wrapper, "empty"},
    {77, empty_wrapper, "empty"},
    {78, empty_wrapper, "empty"},
    {79, empty_wrapper, "empty"},
    // Scenario 11: vm0(L,L), vm1(H,H)
    {80, sorting_wrapper, "sorting_wrapper"},
    {81, qsort_wrapper, "qsort_wrapper"},
    {82, bandwidth_wrapper, "bandwidth_wrapper"},
    {83, disparity_wrapper, "disparity_wrapper"},
    {84, empty_wrapper, "empty"},
    {85, empty_wrapper, "empty"},
    {86, empty_wrapper, "empty"},
    {87, empty_wrapper, "empty"},
    // Scenario 12: vm0(L,L), vm1(L,L)
    {88, sha_wrapper, "sha_wrapper"},
    {89, dijkstra_wrapper, "dijkstra_wrapper"},
    {90, qsort_wrapper, "qsort_wrapper"},
    {91, sorting_wrapper, "sorting_wrapper"},
    {92, empty_wrapper, "empty"},
    {93, empty_wrapper, "empty"},
    {94, empty_wrapper, "empty"},
    {95, empty_wrapper, "empty"},
    // Scenario 13: vm0(H,H), vm2(H,H)
    {96, bandwidth_wrapper, "bandwidth_wrapper"},
    {97, disparity_wrapper, "disparity_wrapper"},
    {98, empty_wrapper, "empty"},
    {99, empty_wrapper, "empty"},
    {100, fft_wrapper, "fft_wrapper"},
    {101, mser_wrapper, "mser_wrapper"},
    {102, empty_wrapper, "empty"},
    {103, empty_wrapper, "empty"},
    // Scenario 14: vm0(H,H), vm2(L,L)
    {104, bandwidth_wrapper, "bandwidth_wrapper"},
    {105, disparity_wrapper, "disparity_wrapper"},
    {106, empty_wrapper, "empty"},
    {107, empty_wrapper, "empty"},
    {108, qsort_wrapper, "qsort_wrapper"},
    {109, sorting_wrapper, "sorting_wrapper"},
    {110, empty_wrapper, "empty"},
    {111, empty_wrapper, "empty"},
    // Scenario 15: vm0(H,L), vm1(L,H)
    {112, bandwidth_wrapper, "bandwidth_wrapper"},
    {113, qsort_wrapper, "qsort_wrapper"},
    {114, sorting_wrapper, "sorting_wrapper"},
    {115, disparity_wrapper, "disparity_wrapper"},
    {116, empty_wrapper, "empty"},
    {117, empty_wrapper, "empty"},
    {118, empty_wrapper, "empty"},
    {119, empty_wrapper, "empty"},
    // Scenario 16: vm0(L,H), vm1(H,L)
    {120, sha_wrapper, "sha_wrapper"},
    {121, mser_wrapper, "mser_wrapper"},
    {122, fft_wrapper, "fft_wrapper"},
    {123, dijkstra_wrapper, "dijkstra_wrapper"},
    {124, empty_wrapper, "empty"},
    {125, empty_wrapper, "empty"},
    {126, empty_wrapper, "empty"},
    {127, empty_wrapper, "empty"},
    // Scenario 17: vm0(H,H), vm3(H,H)
    {128, bandwidth_wrapper, "bandwidth_wrapper"},
    {129, disparity_wrapper, "disparity_wrapper"},
    {130, empty_wrapper, "empty"},
    {131, empty_wrapper, "empty"},
    {132, empty_wrapper, "empty"},
    {133, empty_wrapper, "empty"},
    {134, fft_wrapper, "fft_wrapper"},
    {135, mser_wrapper, "mser_wrapper"},
    // Scenario 18: vm0(H,H), vm3(L,L)
    {136, bandwidth_wrapper, "bandwidth_wrapper"},
    {137, disparity_wrapper, "disparity_wrapper"},
    {138, empty_wrapper, "empty"},
    {139, empty_wrapper, "empty"},
    {140, empty_wrapper, "empty"},
    {141, empty_wrapper, "empty"},
    {142, qsort_wrapper, "qsort_wrapper"},
    {143, sorting_wrapper, "sorting_wrapper"},
    // Scenario 19: vm0(L,L), vm2(H,H)
    {144, qsort_wrapper, "qsort_wrapper"},
    {145, sorting_wrapper, "sorting_wrapper"},
    {146, empty_wrapper, "empty"},
    {147, empty_wrapper, "empty"},
    {148, bandwidth_wrapper, "bandwidth_wrapper"},
    {149, disparity_wrapper, "disparity_wrapper"},
    {150, empty_wrapper, "empty"},
    {151, empty_wrapper, "empty"},
    // Scenario 20: vm0(L,L), vm3(H,H)
    {152, qsort_wrapper, "qsort_wrapper"},
    {153, sorting_wrapper, "sorting_wrapper"},
    {154, empty_wrapper, "empty"},
    {155, empty_wrapper, "empty"},
    {156, empty_wrapper, "empty"},
    {157, empty_wrapper, "empty"},
    {158, bandwidth_wrapper, "bandwidth_wrapper"},
    {159, disparity_wrapper, "disparity_wrapper"},

    // --- Sub-Category: 3 Active VMs (Scenarios 21-32) ---
    // Scenario 21: vm0(H,H), vm1(H,H), vm2(H,H)
    {160, bandwidth_wrapper, "bandwidth_wrapper"},
    {161, disparity_wrapper, "disparity_wrapper"},
    {162, fft_wrapper, "fft_wrapper"},
    {163, mser_wrapper, "mser_wrapper"},
    {164, bandwidth_wrapper, "bandwidth_wrapper"},
    {165, fft_wrapper, "fft_wrapper"},
    {166, empty_wrapper, "empty"},
    {167, empty_wrapper, "empty"},
    // Scenario 22: vm0(L,L), vm1(L,L), vm2(L,L)
    {168, qsort_wrapper, "qsort_wrapper"},
    {169, sorting_wrapper, "sorting_wrapper"},
    {170, sha_wrapper, "sha_wrapper"},
    {171, dijkstra_wrapper, "dijkstra_wrapper"},
    {172, qsort_wrapper, "qsort_wrapper"},
    {173, sha_wrapper, "sha_wrapper"},
    {174, empty_wrapper, "empty"},
    {175, empty_wrapper, "empty"},
    // Scenario 23: vm0(H,H), vm1(H,H), vm2(L,L)
    {176, bandwidth_wrapper, "bandwidth_wrapper"},
    {177, disparity_wrapper, "disparity_wrapper"},
    {178, fft_wrapper, "fft_wrapper"},
    {179, mser_wrapper, "mser_wrapper"},
    {180, qsort_wrapper, "qsort_wrapper"},
    {181, sorting_wrapper, "sorting_wrapper"},
    {182, empty_wrapper, "empty"},
    {183, empty_wrapper, "empty"},
    // Scenario 24: vm0(H,H), vm1(L,L), vm2(H,H)
    {184, bandwidth_wrapper, "bandwidth_wrapper"},
    {185, disparity_wrapper, "disparity_wrapper"},
    {186, qsort_wrapper, "qsort_wrapper"},
    {187, sorting_wrapper, "sorting_wrapper"},
    {188, fft_wrapper, "fft_wrapper"},
    {189, mser_wrapper, "mser_wrapper"},
    {190, empty_wrapper, "empty"},
    {191, empty_wrapper, "empty"},
    // Scenario 25: vm0(L,L), vm1(H,H), vm2(H,H)
    {192, qsort_wrapper, "qsort_wrapper"},
    {193, sorting_wrapper, "sorting_wrapper"},
    {194, bandwidth_wrapper, "bandwidth_wrapper"},
    {195, disparity_wrapper, "disparity_wrapper"},
    {196, fft_wrapper, "fft_wrapper"},
    {197, mser_wrapper, "mser_wrapper"},
    {198, empty_wrapper, "empty"},
    {199, empty_wrapper, "empty"},
    // Scenario 26: vm0(H,L), vm1(L,H), vm2(H,L)
    {200, bandwidth_wrapper, "bandwidth_wrapper"},
    {201, qsort_wrapper, "qsort_wrapper"},
    {202, sorting_wrapper, "sorting_wrapper"},
    {203, mser_wrapper, "mser_wrapper"},
    {204, fft_wrapper, "fft_wrapper"},
    {205, sha_wrapper, "sha_wrapper"},
    {206, empty_wrapper, "empty"},
    {207, empty_wrapper, "empty"},
    // Scenario 27: vm0(H,H), vm1(H,H), vm3(H,H)
    {208, bandwidth_wrapper, "bandwidth_wrapper"},
    {209, disparity_wrapper, "disparity_wrapper"},
    {210, fft_wrapper, "fft_wrapper"},
    {211, mser_wrapper, "mser_wrapper"},
    {212, empty_wrapper, "empty"},
    {213, empty_wrapper, "empty"},
    {214, bandwidth_wrapper, "bandwidth_wrapper"},
    {215, fft_wrapper, "fft_wrapper"},
    // Scenario 28: vm0(H,H), vm1(L,L), vm3(L,L)
    {216, bandwidth_wrapper, "bandwidth_wrapper"},
    {217, disparity_wrapper, "disparity_wrapper"},
    {218, qsort_wrapper, "qsort_wrapper"},
    {219, sorting_wrapper, "sorting_wrapper"},
    {220, empty_wrapper, "empty"},
    {221, empty_wrapper, "empty"},
    {222, sha_wrapper, "sha_wrapper"},
    {223, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 29: vm0(L,L), vm2(H,H), vm3(H,H)
    {224, qsort_wrapper, "qsort_wrapper"},
    {225, sorting_wrapper, "sorting_wrapper"},
    {226, empty_wrapper, "empty"},
    {227, empty_wrapper, "empty"},
    {228, bandwidth_wrapper, "bandwidth_wrapper"},
    {229, disparity_wrapper, "disparity_wrapper"},
    {230, fft_wrapper, "fft_wrapper"},
    {231, mser_wrapper, "mser_wrapper"},
    // Scenario 30: vm1(H,H), vm2(H,H), vm3(H,H)
    {232, empty_wrapper, "empty"},
    {233, empty_wrapper, "empty"},
    {234, bandwidth_wrapper, "bandwidth_wrapper"},
    {235, disparity_wrapper, "disparity_wrapper"},
    {236, fft_wrapper, "fft_wrapper"},
    {237, mser_wrapper, "mser_wrapper"},
    {238, bandwidth_wrapper, "bandwidth_wrapper"},
    {239, fft_wrapper, "fft_wrapper"},
    // Scenario 31: vm0(L,L), vm1(L,L), vm3(H,H)
    {240, qsort_wrapper, "qsort_wrapper"},
    {241, sha_wrapper, "sha_wrapper"},
    {242, sorting_wrapper, "sorting_wrapper"},
    {243, dijkstra_wrapper, "dijkstra_wrapper"},
    {244, empty_wrapper, "empty"},
    {245, empty_wrapper, "empty"},
    {246, bandwidth_wrapper, "bandwidth_wrapper"},
    {247, mser_wrapper, "mser_wrapper"},
    // Scenario 32: vm0(H,L), vm2(L,H), vm3(H,L)
    {248, bandwidth_wrapper, "bandwidth_wrapper"},
    {249, qsort_wrapper, "qsort_wrapper"},
    {250, empty_wrapper, "empty"},
    {251, empty_wrapper, "empty"},
    {252, sorting_wrapper, "sorting_wrapper"},
    {253, mser_wrapper, "mser_wrapper"},
    {254, fft_wrapper, "fft_wrapper"},
    {255, sha_wrapper, "sha_wrapper"},

    // --- Sub-Category: 4 Active VMs (Scenarios 33-40) ---
    // Scenario 33: 2 VMs (H,H), 2 VMs (L,L)
    {256, bandwidth_wrapper, "bandwidth_wrapper"},
    {257, disparity_wrapper, "disparity_wrapper"},
    {258, fft_wrapper, "fft_wrapper"},
    {259, mser_wrapper, "mser_wrapper"},
    {260, qsort_wrapper, "qsort_wrapper"},
    {261, sorting_wrapper, "sorting_wrapper"},
    {262, sha_wrapper, "sha_wrapper"},
    {263, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 34: 2 VMs (H,H), 2 VMs (L,L) - swapped
    {264, qsort_wrapper, "qsort_wrapper"},
    {265, sorting_wrapper, "sorting_wrapper"},
    {266, sha_wrapper, "sha_wrapper"},
    {267, dijkstra_wrapper, "dijkstra_wrapper"},
    {268, bandwidth_wrapper, "bandwidth_wrapper"},
    {269, disparity_wrapper, "disparity_wrapper"},
    {270, fft_wrapper, "fft_wrapper"},
    {271, mser_wrapper, "mser_wrapper"},
    // Scenario 35: 3 VMs (H,H), 1 VM (L,L)
    {272, bandwidth_wrapper, "bandwidth_wrapper"},
    {273, disparity_wrapper, "disparity_wrapper"},
    {274, fft_wrapper, "fft_wrapper"},
    {275, mser_wrapper, "mser_wrapper"},
    {276, bandwidth_wrapper, "bandwidth_wrapper"},
    {277, fft_wrapper, "fft_wrapper"},
    {278, qsort_wrapper, "qsort_wrapper"},
    {279, sorting_wrapper, "sorting_wrapper"},
    // Scenario 36: 1 VM (H,H), 3 VMs (L,L)
    {280, bandwidth_wrapper, "bandwidth_wrapper"},
    {281, disparity_wrapper, "disparity_wrapper"},
    {282, qsort_wrapper, "qsort_wrapper"},
    {283, sorting_wrapper, "sorting_wrapper"},
    {284, sha_wrapper, "sha_wrapper"},
    {285, dijkstra_wrapper, "dijkstra_wrapper"},
    {286, qsort_wrapper, "qsort_wrapper"},
    {287, sha_wrapper, "sha_wrapper"},
    // Scenario 37: All VMs (H,L)
    {288, bandwidth_wrapper, "bandwidth_wrapper"},
    {289, qsort_wrapper, "qsort_wrapper"},
    {290, fft_wrapper, "fft_wrapper"},
    {291, sorting_wrapper, "sorting_wrapper"},
    {292, disparity_wrapper, "disparity_wrapper"},
    {293, sha_wrapper, "sha_wrapper"},
    {294, mser_wrapper, "mser_wrapper"},
    {295, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 38: All VMs (L,H)
    {296, qsort_wrapper, "qsort_wrapper"},
    {297, bandwidth_wrapper, "bandwidth_wrapper"},
    {298, sorting_wrapper, "sorting_wrapper"},
    {299, fft_wrapper, "fft_wrapper"},
    {300, sha_wrapper, "sha_wrapper"},
    {301, disparity_wrapper, "disparity_wrapper"},
    {302, dijkstra_wrapper, "dijkstra_wrapper"},
    {303, mser_wrapper, "mser_wrapper"},
    // Scenario 39: Alternating (H,H) and (L,L)
    {304, bandwidth_wrapper, "bandwidth_wrapper"},
    {305, disparity_wrapper, "disparity_wrapper"},
    {306, qsort_wrapper, "qsort_wrapper"},
    {307, sorting_wrapper, "sorting_wrapper"},
    {308, fft_wrapper, "fft_wrapper"},
    {309, mser_wrapper, "mser_wrapper"},
    {310, sha_wrapper, "sha_wrapper"},
    {311, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 40: Alternating (L,L) and (H,H)
    {312, qsort_wrapper, "qsort_wrapper"},
    {313, sorting_wrapper, "sorting_wrapper"},
    {314, bandwidth_wrapper, "bandwidth_wrapper"},
    {315, disparity_wrapper, "disparity_wrapper"},
    {316, sha_wrapper, "sha_wrapper"},
    {317, dijkstra_wrapper, "dijkstra_wrapper"},
    {318, fft_wrapper, "fft_wrapper"},
    {319, mser_wrapper, "mser_wrapper"},

    // ===========================================================================
    // ==   Category B: Chaotic Mix & Permutation Scenarios (Scenarios 41-104)
    // ==
    // ===========================================================================

    // --- Sub-Category: Full Permutations (Scenarios 41-72) ---
    // Scenario 41
    {320, bandwidth_wrapper, "bandwidth_wrapper"},
    {321, disparity_wrapper, "disparity_wrapper"},
    {322, fft_wrapper, "fft_wrapper"},
    {323, mser_wrapper, "mser_wrapper"},
    {324, qsort_wrapper, "qsort_wrapper"},
    {325, sorting_wrapper, "sorting_wrapper"},
    {326, sha_wrapper, "sha_wrapper"},
    {327, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 42
    {328, dijkstra_wrapper, "dijkstra_wrapper"},
    {329, sha_wrapper, "sha_wrapper"},
    {330, sorting_wrapper, "sorting_wrapper"},
    {331, qsort_wrapper, "qsort_wrapper"},
    {332, mser_wrapper, "mser_wrapper"},
    {333, fft_wrapper, "fft_wrapper"},
    {334, disparity_wrapper, "disparity_wrapper"},
    {335, bandwidth_wrapper, "bandwidth_wrapper"},
    // Scenario 43
    {336, fft_wrapper, "fft_wrapper"},
    {337, qsort_wrapper, "qsort_wrapper"},
    {338, bandwidth_wrapper, "bandwidth_wrapper"},
    {339, sha_wrapper, "sha_wrapper"},
    {340, disparity_wrapper, "disparity_wrapper"},
    {341, sorting_wrapper, "sorting_wrapper"},
    {342, mser_wrapper, "mser_wrapper"},
    {343, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 44
    {344, mser_wrapper, "mser_wrapper"},
    {345, dijkstra_wrapper, "dijkstra_wrapper"},
    {346, sorting_wrapper, "sorting_wrapper"},
    {347, bandwidth_wrapper, "bandwidth_wrapper"},
    {348, sha_wrapper, "sha_wrapper"},
    {349, disparity_wrapper, "disparity_wrapper"},
    {350, qsort_wrapper, "qsort_wrapper"},
    {351, fft_wrapper, "fft_wrapper"},
    // Scenario 45
    {352, sorting_wrapper, "sorting_wrapper"},
    {353, bandwidth_wrapper, "bandwidth_wrapper"},
    {354, sha_wrapper, "sha_wrapper"},
    {355, fft_wrapper, "fft_wrapper"},
    {356, dijkstra_wrapper, "dijkstra_wrapper"},
    {357, qsort_wrapper, "qsort_wrapper"},
    {358, disparity_wrapper, "disparity_wrapper"},
    {359, mser_wrapper, "mser_wrapper"},
    // Scenario 46
    {360, qsort_wrapper, "qsort_wrapper"},
    {361, mser_wrapper, "mser_wrapper"},
    {362, dijkstra_wrapper, "dijkstra_wrapper"},
    {363, disparity_wrapper, "disparity_wrapper"},
    {364, fft_wrapper, "fft_wrapper"},
    {365, sha_wrapper, "sha_wrapper"},
    {366, bandwidth_wrapper, "bandwidth_wrapper"},
    {367, sorting_wrapper, "sorting_wrapper"},
    // Scenario 47
    {368, disparity_wrapper, "disparity_wrapper"},
    {369, sorting_wrapper, "sorting_wrapper"},
    {370, fft_wrapper, "fft_wrapper"},
    {371, dijkstra_wrapper, "dijkstra_wrapper"},
    {372, bandwidth_wrapper, "bandwidth_wrapper"},
    {373, mser_wrapper, "mser_wrapper"},
    {374, qsort_wrapper, "qsort_wrapper"},
    {375, sha_wrapper, "sha_wrapper"},
    // Scenario 48
    {376, sha_wrapper, "sha_wrapper"},
    {377, fft_wrapper, "fft_wrapper"},
    {378, bandwidth_wrapper, "bandwidth_wrapper"},
    {379, qsort_wrapper, "qsort_wrapper"},
    {380, mser_wrapper, "mser_wrapper"},
    {381, sorting_wrapper, "sorting_wrapper"},
    {382, dijkstra_wrapper, "dijkstra_wrapper"},
    {383, disparity_wrapper, "disparity_wrapper"},
    // Scenario 49
    {384, bandwidth_wrapper, "bandwidth_wrapper"},
    {385, qsort_wrapper, "qsort_wrapper"},
    {386, mser_wrapper, "mser_wrapper"},
    {387, sorting_wrapper, "sorting_wrapper"},
    {388, fft_wrapper, "fft_wrapper"},
    {389, sha_wrapper, "sha_wrapper"},
    {390, dijkstra_wrapper, "dijkstra_wrapper"},
    {391, disparity_wrapper, "disparity_wrapper"},
    // Scenario 50
    {392, disparity_wrapper, "disparity_wrapper"},
    {393, dijkstra_wrapper, "dijkstra_wrapper"},
    {394, sha_wrapper, "sha_wrapper"},
    {395, fft_wrapper, "fft_wrapper"},
    {396, sorting_wrapper, "sorting_wrapper"},
    {397, mser_wrapper, "mser_wrapper"},
    {398, qsort_wrapper, "qsort_wrapper"},
    {399, bandwidth_wrapper, "bandwidth_wrapper"},
    // Scenario 51
    {400, fft_wrapper, "fft_wrapper"},
    {401, sorting_wrapper, "sorting_wrapper"},
    {402, bandwidth_wrapper, "bandwidth_wrapper"},
    {403, dijkstra_wrapper, "dijkstra_wrapper"},
    {404, mser_wrapper, "mser_wrapper"},
    {405, sha_wrapper, "sha_wrapper"},
    {406, qsort_wrapper, "qsort_wrapper"},
    {407, disparity_wrapper, "disparity_wrapper"},
    // Scenario 52
    {408, mser_wrapper, "mser_wrapper"},
    {409, qsort_wrapper, "qsort_wrapper"},
    {410, disparity_wrapper, "disparity_wrapper"},
    {411, sha_wrapper, "sha_wrapper"},
    {412, bandwidth_wrapper, "bandwidth_wrapper"},
    {413, dijkstra_wrapper, "dijkstra_wrapper"},
    {414, sorting_wrapper, "sorting_wrapper"},
    {415, fft_wrapper, "fft_wrapper"},
    // Scenario 53
    {416, sorting_wrapper, "sorting_wrapper"},
    {417, disparity_wrapper, "disparity_wrapper"},
    {418, mser_wrapper, "mser_wrapper"},
    {419, bandwidth_wrapper, "bandwidth_wrapper"},
    {420, qsort_wrapper, "qsort_wrapper"},
    {421, fft_wrapper, "fft_wrapper"},
    {422, dijkstra_wrapper, "dijkstra_wrapper"},
    {423, sha_wrapper, "sha_wrapper"},
    // Scenario 54
    {424, qsort_wrapper, "qsort_wrapper"},
    {425, bandwidth_wrapper, "bandwidth_wrapper"},
    {426, sha_wrapper, "sha_wrapper"},
    {427, fft_wrapper, "fft_wrapper"},
    {428, dijkstra_wrapper, "dijkstra_wrapper"},
    {429, disparity_wrapper, "disparity_wrapper"},
    {430, mser_wrapper, "mser_wrapper"},
    {431, sorting_wrapper, "sorting_wrapper"},
    // Scenario 55
    {432, disparity_wrapper, "disparity_wrapper"},
    {433, mser_wrapper, "mser_wrapper"},
    {434, dijkstra_wrapper, "dijkstra_wrapper"},
    {435, sorting_wrapper, "sorting_wrapper"},
    {436, sha_wrapper, "sha_wrapper"},
    {437, bandwidth_wrapper, "bandwidth_wrapper"},
    {438, fft_wrapper, "fft_wrapper"},
    {439, qsort_wrapper, "qsort_wrapper"},
    // Scenario 56
    {440, sha_wrapper, "sha_wrapper"},
    {441, sorting_wrapper, "sorting_wrapper"},
    {442, qsort_wrapper, "qsort_wrapper"},
    {443, mser_wrapper, "mser_wrapper"},
    {444, bandwidth_wrapper, "bandwidth_wrapper"},
    {445, fft_wrapper, "fft_wrapper"},
    {446, disparity_wrapper, "disparity_wrapper"},
    {447, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 57
    {448, bandwidth_wrapper, "bandwidth_wrapper"},
    {449, fft_wrapper, "fft_wrapper"},
    {450, sha_wrapper, "sha_wrapper"},
    {451, qsort_wrapper, "qsort_wrapper"},
    {452, disparity_wrapper, "disparity_wrapper"},
    {453, mser_wrapper, "mser_wrapper"},
    {454, sorting_wrapper, "sorting_wrapper"},
    {455, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 58
    {456, disparity_wrapper, "disparity_wrapper"},
    {457, dijkstra_wrapper, "dijkstra_wrapper"},
    {458, mser_wrapper, "mser_wrapper"},
    {459, sorting_wrapper, "sorting_wrapper"},
    {460, fft_wrapper, "fft_wrapper"},
    {461, bandwidth_wrapper, "bandwidth_wrapper"},
    {462, qsort_wrapper, "qsort_wrapper"},
    {463, sha_wrapper, "sha_wrapper"},
    // Scenario 59
    {464, fft_wrapper, "fft_wrapper"},
    {465, sha_wrapper, "sha_wrapper"},
    {466, dijkstra_wrapper, "dijkstra_wrapper"},
    {467, bandwidth_wrapper, "bandwidth_wrapper"},
    {468, qsort_wrapper, "qsort_wrapper"},
    {469, mser_wrapper, "mser_wrapper"},
    {470, sorting_wrapper, "sorting_wrapper"},
    {471, disparity_wrapper, "disparity_wrapper"},
    // Scenario 60
    {472, mser_wrapper, "mser_wrapper"},
    {473, sorting_wrapper, "sorting_wrapper"},
    {474, qsort_wrapper, "qsort_wrapper"},
    {475, disparity_wrapper, "disparity_wrapper"},
    {476, bandwidth_wrapper, "bandwidth_wrapper"},
    {477, dijkstra_wrapper, "dijkstra_wrapper"},
    {478, sha_wrapper, "sha_wrapper"},
    {479, fft_wrapper, "fft_wrapper"},
    // Scenario 61
    {480, sorting_wrapper, "sorting_wrapper"},
    {481, dijkstra_wrapper, "dijkstra_wrapper"},
    {482, fft_wrapper, "fft_wrapper"},
    {483, sha_wrapper, "sha_wrapper"},
    {484, disparity_wrapper, "disparity_wrapper"},
    {485, qsort_wrapper, "qsort_wrapper"},
    {486, mser_wrapper, "mser_wrapper"},
    {487, bandwidth_wrapper, "bandwidth_wrapper"},
    // Scenario 62
    {488, qsort_wrapper, "qsort_wrapper"},
    {489, sha_wrapper, "sha_wrapper"},
    {490, bandwidth_wrapper, "bandwidth_wrapper"},
    {491, mser_wrapper, "mser_wrapper"},
    {492, fft_wrapper, "fft_wrapper"},
    {493, disparity_wrapper, "disparity_wrapper"},
    {494, dijkstra_wrapper, "dijkstra_wrapper"},
    {495, sorting_wrapper, "sorting_wrapper"},
    // Scenario 63
    {496, disparity_wrapper, "disparity_wrapper"},
    {497, bandwidth_wrapper, "bandwidth_wrapper"},
    {498, sorting_wrapper, "sorting_wrapper"},
    {499, qsort_wrapper, "qsort_wrapper"},
    {500, mser_wrapper, "mser_wrapper"},
    {501, dijkstra_wrapper, "dijkstra_wrapper"},
    {502, fft_wrapper, "fft_wrapper"},
    {503, sha_wrapper, "sha_wrapper"},
    // Scenario 64
    {504, sha_wrapper, "sha_wrapper"},
    {505, mser_wrapper, "mser_wrapper"},
    {506, dijkstra_wrapper, "dijkstra_wrapper"},
    {507, fft_wrapper, "fft_wrapper"},
    {508, sorting_wrapper, "sorting_wrapper"},
    {509, bandwidth_wrapper, "bandwidth_wrapper"},
    {510, qsort_wrapper, "qsort_wrapper"},
    {511, disparity_wrapper, "disparity_wrapper"},
    // Scenario 65
    {512, bandwidth_wrapper, "bandwidth_wrapper"},
    {513, sha_wrapper, "sha_wrapper"},
    {514, qsort_wrapper, "qsort_wrapper"},
    {515, mser_wrapper, "mser_wrapper"},
    {516, disparity_wrapper, "disparity_wrapper"},
    {517, fft_wrapper, "fft_wrapper"},
    {518, dijkstra_wrapper, "dijkstra_wrapper"},
    {519, sorting_wrapper, "sorting_wrapper"},
    // Scenario 66
    {520, disparity_wrapper, "disparity_wrapper"},
    {521, qsort_wrapper, "qsort_wrapper"},
    {522, fft_wrapper, "fft_wrapper"},
    {523, dijkstra_wrapper, "dijkstra_wrapper"},
    {524, bandwidth_wrapper, "bandwidth_wrapper"},
    {525, sorting_wrapper, "sorting_wrapper"},
    {526, sha_wrapper, "sha_wrapper"},
    {527, mser_wrapper, "mser_wrapper"},
    // Scenario 67
    {528, fft_wrapper, "fft_wrapper"},
    {529, mser_wrapper, "mser_wrapper"},
    {530, sorting_wrapper, "sorting_wrapper"},
    {531, bandwidth_wrapper, "bandwidth_wrapper"},
    {532, qsort_wrapper, "qsort_wrapper"},
    {533, dijkstra_wrapper, "dijkstra_wrapper"},
    {534, disparity_wrapper, "disparity_wrapper"},
    {535, sha_wrapper, "sha_wrapper"},
    // Scenario 68
    {536, mser_wrapper, "mser_wrapper"},
    {537, bandwidth_wrapper, "bandwidth_wrapper"},
    {538, dijkstra_wrapper, "dijkstra_wrapper"},
    {539, qsort_wrapper, "qsort_wrapper"},
    {540, sha_wrapper, "sha_wrapper"},
    {541, fft_wrapper, "fft_wrapper"},
    {542, sorting_wrapper, "sorting_wrapper"},
    {543, disparity_wrapper, "disparity_wrapper"},
    // Scenario 69
    {544, sorting_wrapper, "sorting_wrapper"},
    {545, fft_wrapper, "fft_wrapper"},
    {546, bandwidth_wrapper, "bandwidth_wrapper"},
    {547, sha_wrapper, "sha_wrapper"},
    {548, dijkstra_wrapper, "dijkstra_wrapper"},
    {549, disparity_wrapper, "disparity_wrapper"},
    {550, mser_wrapper, "mser_wrapper"},
    {551, qsort_wrapper, "qsort_wrapper"},
    // Scenario 70
    {552, qsort_wrapper, "qsort_wrapper"},
    {553, dijkstra_wrapper, "dijkstra_wrapper"},
    {554, mser_wrapper, "mser_wrapper"},
    {555, disparity_wrapper, "disparity_wrapper"},
    {556, fft_wrapper, "fft_wrapper"},
    {557, bandwidth_wrapper, "bandwidth_wrapper"},
    {558, sha_wrapper, "sha_wrapper"},
    {559, sorting_wrapper, "sorting_wrapper"},
    // Scenario 71
    {560, disparity_wrapper, "disparity_wrapper"},
    {561, sorting_wrapper, "sorting_wrapper"},
    {562, sha_wrapper, "sha_wrapper"},
    {563, qsort_wrapper, "qsort_wrapper"},
    {564, mser_wrapper, "mser_wrapper"},
    {565, fft_wrapper, "fft_wrapper"},
    {566, bandwidth_wrapper, "bandwidth_wrapper"},
    {567, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 72
    {568, sha_wrapper, "sha_wrapper"},
    {569, bandwidth_wrapper, "bandwidth_wrapper"},
    {570, qsort_wrapper, "qsort_wrapper"},
    {571, mser_wrapper, "mser_wrapper"},
    {572, fft_wrapper, "fft_wrapper"},
    {573, sorting_wrapper, "sorting_wrapper"},
    {574, disparity_wrapper, "disparity_wrapper"},
    {575, dijkstra_wrapper, "dijkstra_wrapper"},

    // --- Sub-Category: Mixed H/L Permutations (Scenarios 73-104) ---
    // Scenario 73: HHHHLLLL
    {576, bandwidth_wrapper, "bandwidth_wrapper"},
    {577, disparity_wrapper, "disparity_wrapper"},
    {578, fft_wrapper, "fft_wrapper"},
    {579, mser_wrapper, "mser_wrapper"},
    {580, qsort_wrapper, "qsort_wrapper"},
    {581, sorting_wrapper, "sorting_wrapper"},
    {582, sha_wrapper, "sha_wrapper"},
    {583, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 74: LLLLHHHH
    {584, qsort_wrapper, "qsort_wrapper"},
    {585, sorting_wrapper, "sorting_wrapper"},
    {586, sha_wrapper, "sha_wrapper"},
    {587, dijkstra_wrapper, "dijkstra_wrapper"},
    {588, bandwidth_wrapper, "bandwidth_wrapper"},
    {589, disparity_wrapper, "disparity_wrapper"},
    {590, fft_wrapper, "fft_wrapper"},
    {591, mser_wrapper, "mser_wrapper"},
    // Scenario 75: HLHLHLHL
    {592, bandwidth_wrapper, "bandwidth_wrapper"},
    {593, qsort_wrapper, "qsort_wrapper"},
    {594, disparity_wrapper, "disparity_wrapper"},
    {595, sorting_wrapper, "sorting_wrapper"},
    {596, fft_wrapper, "fft_wrapper"},
    {597, sha_wrapper, "sha_wrapper"},
    {598, mser_wrapper, "mser_wrapper"},
    {599, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 76: LHLHLHLH
    {600, qsort_wrapper, "qsort_wrapper"},
    {601, bandwidth_wrapper, "bandwidth_wrapper"},
    {602, sorting_wrapper, "sorting_wrapper"},
    {603, disparity_wrapper, "disparity_wrapper"},
    {604, sha_wrapper, "sha_wrapper"},
    {605, fft_wrapper, "fft_wrapper"},
    {606, dijkstra_wrapper, "dijkstra_wrapper"},
    {607, mser_wrapper, "mser_wrapper"},
    // Scenario 77: HHLLHHLL
    {608, bandwidth_wrapper, "bandwidth_wrapper"},
    {609, disparity_wrapper, "disparity_wrapper"},
    {610, qsort_wrapper, "qsort_wrapper"},
    {611, sorting_wrapper, "sorting_wrapper"},
    {612, fft_wrapper, "fft_wrapper"},
    {613, mser_wrapper, "mser_wrapper"},
    {614, sha_wrapper, "sha_wrapper"},
    {615, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 78: LLHHLLHH
    {616, qsort_wrapper, "qsort_wrapper"},
    {617, sorting_wrapper, "sorting_wrapper"},
    {618, bandwidth_wrapper, "bandwidth_wrapper"},
    {619, disparity_wrapper, "disparity_wrapper"},
    {620, sha_wrapper, "sha_wrapper"},
    {621, dijkstra_wrapper, "dijkstra_wrapper"},
    {622, fft_wrapper, "fft_wrapper"},
    {623, mser_wrapper, "mser_wrapper"},
    // Scenario 79
    {624, bandwidth_wrapper, "bandwidth_wrapper"},
    {625, fft_wrapper, "fft_wrapper"},
    {626, qsort_wrapper, "qsort_wrapper"},
    {627, sha_wrapper, "sha_wrapper"},
    {628, disparity_wrapper, "disparity_wrapper"},
    {629, mser_wrapper, "mser_wrapper"},
    {630, sorting_wrapper, "sorting_wrapper"},
    {631, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 80
    {632, dijkstra_wrapper, "dijkstra_wrapper"},
    {633, mser_wrapper, "mser_wrapper"},
    {634, sorting_wrapper, "sorting_wrapper"},
    {635, disparity_wrapper, "disparity_wrapper"},
    {636, sha_wrapper, "sha_wrapper"},
    {637, fft_wrapper, "fft_wrapper"},
    {638, qsort_wrapper, "qsort_wrapper"},
    {639, bandwidth_wrapper, "bandwidth_wrapper"},
    // Scenario 81
    {640, fft_wrapper, "fft_wrapper"},
    {641, qsort_wrapper, "qsort_wrapper"},
    {642, bandwidth_wrapper, "bandwidth_wrapper"},
    {643, sha_wrapper, "sha_wrapper"},
    {644, disparity_wrapper, "disparity_wrapper"},
    {645, sorting_wrapper, "sorting_wrapper"},
    {646, mser_wrapper, "mser_wrapper"},
    {647, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 82
    {648, mser_wrapper, "mser_wrapper"},
    {649, dijkstra_wrapper, "dijkstra_wrapper"},
    {650, sorting_wrapper, "sorting_wrapper"},
    {651, bandwidth_wrapper, "bandwidth_wrapper"},
    {652, sha_wrapper, "sha_wrapper"},
    {653, disparity_wrapper, "disparity_wrapper"},
    {654, qsort_wrapper, "qsort_wrapper"},
    {655, fft_wrapper, "fft_wrapper"},
    // Scenario 83
    {656, sorting_wrapper, "sorting_wrapper"},
    {657, bandwidth_wrapper, "bandwidth_wrapper"},
    {658, sha_wrapper, "sha_wrapper"},
    {659, fft_wrapper, "fft_wrapper"},
    {660, dijkstra_wrapper, "dijkstra_wrapper"},
    {661, qsort_wrapper, "qsort_wrapper"},
    {662, disparity_wrapper, "disparity_wrapper"},
    {663, mser_wrapper, "mser_wrapper"},
    // Scenario 84
    {664, qsort_wrapper, "qsort_wrapper"},
    {665, mser_wrapper, "mser_wrapper"},
    {666, dijkstra_wrapper, "dijkstra_wrapper"},
    {667, disparity_wrapper, "disparity_wrapper"},
    {668, fft_wrapper, "fft_wrapper"},
    {669, sha_wrapper, "sha_wrapper"},
    {670, bandwidth_wrapper, "bandwidth_wrapper"},
    {671, sorting_wrapper, "sorting_wrapper"},
    // Scenario 85
    {672, disparity_wrapper, "disparity_wrapper"},
    {673, sorting_wrapper, "sorting_wrapper"},
    {674, fft_wrapper, "fft_wrapper"},
    {675, dijkstra_wrapper, "dijkstra_wrapper"},
    {676, bandwidth_wrapper, "bandwidth_wrapper"},
    {677, mser_wrapper, "mser_wrapper"},
    {678, qsort_wrapper, "qsort_wrapper"},
    {679, sha_wrapper, "sha_wrapper"},
    // Scenario 86
    {680, sha_wrapper, "sha_wrapper"},
    {681, fft_wrapper, "fft_wrapper"},
    {682, bandwidth_wrapper, "bandwidth_wrapper"},
    {683, qsort_wrapper, "qsort_wrapper"},
    {684, mser_wrapper, "mser_wrapper"},
    {685, sorting_wrapper, "sorting_wrapper"},
    {686, dijkstra_wrapper, "dijkstra_wrapper"},
    {687, disparity_wrapper, "disparity_wrapper"},
    // Scenario 87
    {688, bandwidth_wrapper, "bandwidth_wrapper"},
    {689, qsort_wrapper, "qsort_wrapper"},
    {690, mser_wrapper, "mser_wrapper"},
    {691, sorting_wrapper, "sorting_wrapper"},
    {692, fft_wrapper, "fft_wrapper"},
    {693, sha_wrapper, "sha_wrapper"},
    {694, dijkstra_wrapper, "dijkstra_wrapper"},
    {695, disparity_wrapper, "disparity_wrapper"},
    // Scenario 88
    {696, disparity_wrapper, "disparity_wrapper"},
    {697, dijkstra_wrapper, "dijkstra_wrapper"},
    {698, sha_wrapper, "sha_wrapper"},
    {699, fft_wrapper, "fft_wrapper"},
    {700, sorting_wrapper, "sorting_wrapper"},
    {701, mser_wrapper, "mser_wrapper"},
    {702, qsort_wrapper, "qsort_wrapper"},
    {703, bandwidth_wrapper, "bandwidth_wrapper"},
    // Scenario 89
    {704, fft_wrapper, "fft_wrapper"},
    {705, sorting_wrapper, "sorting_wrapper"},
    {706, bandwidth_wrapper, "bandwidth_wrapper"},
    {707, dijkstra_wrapper, "dijkstra_wrapper"},
    {708, mser_wrapper, "mser_wrapper"},
    {709, sha_wrapper, "sha_wrapper"},
    {710, qsort_wrapper, "qsort_wrapper"},
    {711, disparity_wrapper, "disparity_wrapper"},
    // Scenario 90
    {712, mser_wrapper, "mser_wrapper"},
    {713, qsort_wrapper, "qsort_wrapper"},
    {714, disparity_wrapper, "disparity_wrapper"},
    {715, sha_wrapper, "sha_wrapper"},
    {716, bandwidth_wrapper, "bandwidth_wrapper"},
    {717, dijkstra_wrapper, "dijkstra_wrapper"},
    {718, sorting_wrapper, "sorting_wrapper"},
    {719, fft_wrapper, "fft_wrapper"},
    // Scenario 91
    {720, sorting_wrapper, "sorting_wrapper"},
    {721, disparity_wrapper, "disparity_wrapper"},
    {722, mser_wrapper, "mser_wrapper"},
    {723, bandwidth_wrapper, "bandwidth_wrapper"},
    {724, qsort_wrapper, "qsort_wrapper"},
    {725, fft_wrapper, "fft_wrapper"},
    {726, dijkstra_wrapper, "dijkstra_wrapper"},
    {727, sha_wrapper, "sha_wrapper"},
    // Scenario 92
    {728, qsort_wrapper, "qsort_wrapper"},
    {729, bandwidth_wrapper, "bandwidth_wrapper"},
    {730, sha_wrapper, "sha_wrapper"},
    {731, fft_wrapper, "fft_wrapper"},
    {732, dijkstra_wrapper, "dijkstra_wrapper"},
    {733, disparity_wrapper, "disparity_wrapper"},
    {734, mser_wrapper, "mser_wrapper"},
    {735, sorting_wrapper, "sorting_wrapper"},
    // Scenario 93
    {736, disparity_wrapper, "disparity_wrapper"},
    {737, mser_wrapper, "mser_wrapper"},
    {738, dijkstra_wrapper, "dijkstra_wrapper"},
    {739, sorting_wrapper, "sorting_wrapper"},
    {740, sha_wrapper, "sha_wrapper"},
    {741, bandwidth_wrapper, "bandwidth_wrapper"},
    {742, fft_wrapper, "fft_wrapper"},
    {743, qsort_wrapper, "qsort_wrapper"},
    // Scenario 94
    {744, sha_wrapper, "sha_wrapper"},
    {745, sorting_wrapper, "sorting_wrapper"},
    {746, qsort_wrapper, "qsort_wrapper"},
    {747, mser_wrapper, "mser_wrapper"},
    {748, bandwidth_wrapper, "bandwidth_wrapper"},
    {749, fft_wrapper, "fft_wrapper"},
    {750, disparity_wrapper, "disparity_wrapper"},
    {751, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 95
    {752, bandwidth_wrapper, "bandwidth_wrapper"},
    {753, fft_wrapper, "fft_wrapper"},
    {754, sha_wrapper, "sha_wrapper"},
    {755, qsort_wrapper, "qsort_wrapper"},
    {756, disparity_wrapper, "disparity_wrapper"},
    {757, mser_wrapper, "mser_wrapper"},
    {758, sorting_wrapper, "sorting_wrapper"},
    {759, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 96
    {760, disparity_wrapper, "disparity_wrapper"},
    {761, dijkstra_wrapper, "dijkstra_wrapper"},
    {762, mser_wrapper, "mser_wrapper"},
    {763, sorting_wrapper, "sorting_wrapper"},
    {764, fft_wrapper, "fft_wrapper"},
    {765, bandwidth_wrapper, "bandwidth_wrapper"},
    {766, qsort_wrapper, "qsort_wrapper"},
    {767, sha_wrapper, "sha_wrapper"},
    // Scenario 97
    {768, fft_wrapper, "fft_wrapper"},
    {769, sha_wrapper, "sha_wrapper"},
    {770, dijkstra_wrapper, "dijkstra_wrapper"},
    {771, bandwidth_wrapper, "bandwidth_wrapper"},
    {772, qsort_wrapper, "qsort_wrapper"},
    {773, mser_wrapper, "mser_wrapper"},
    {774, sorting_wrapper, "sorting_wrapper"},
    {775, disparity_wrapper, "disparity_wrapper"},
    // Scenario 98
    {776, mser_wrapper, "mser_wrapper"},
    {777, sorting_wrapper, "sorting_wrapper"},
    {778, qsort_wrapper, "qsort_wrapper"},
    {779, disparity_wrapper, "disparity_wrapper"},
    {780, bandwidth_wrapper, "bandwidth_wrapper"},
    {781, dijkstra_wrapper, "dijkstra_wrapper"},
    {782, sha_wrapper, "sha_wrapper"},
    {783, fft_wrapper, "fft_wrapper"},
    // Scenario 99
    {784, sorting_wrapper, "sorting_wrapper"},
    {785, dijkstra_wrapper, "dijkstra_wrapper"},
    {786, fft_wrapper, "fft_wrapper"},
    {787, sha_wrapper, "sha_wrapper"},
    {788, disparity_wrapper, "disparity_wrapper"},
    {789, qsort_wrapper, "qsort_wrapper"},
    {790, mser_wrapper, "mser_wrapper"},
    {791, bandwidth_wrapper, "bandwidth_wrapper"},
    // Scenario 100
    {792, qsort_wrapper, "qsort_wrapper"},
    {793, sha_wrapper, "sha_wrapper"},
    {794, bandwidth_wrapper, "bandwidth_wrapper"},
    {795, mser_wrapper, "mser_wrapper"},
    {796, fft_wrapper, "fft_wrapper"},
    {797, disparity_wrapper, "disparity_wrapper"},
    {798, dijkstra_wrapper, "dijkstra_wrapper"},
    {799, sorting_wrapper, "sorting_wrapper"},
    // Scenario 101
    {800, disparity_wrapper, "disparity_wrapper"},
    {801, bandwidth_wrapper, "bandwidth_wrapper"},
    {802, sorting_wrapper, "sorting_wrapper"},
    {803, qsort_wrapper, "qsort_wrapper"},
    {804, mser_wrapper, "mser_wrapper"},
    {805, dijkstra_wrapper, "dijkstra_wrapper"},
    {806, fft_wrapper, "fft_wrapper"},
    {807, sha_wrapper, "sha_wrapper"},
    // Scenario 102
    {808, sha_wrapper, "sha_wrapper"},
    {809, mser_wrapper, "mser_wrapper"},
    {810, dijkstra_wrapper, "dijkstra_wrapper"},
    {811, fft_wrapper, "fft_wrapper"},
    {812, sorting_wrapper, "sorting_wrapper"},
    {813, bandwidth_wrapper, "bandwidth_wrapper"},
    {814, qsort_wrapper, "qsort_wrapper"},
    {815, disparity_wrapper, "disparity_wrapper"},
    // Scenario 103
    {816, bandwidth_wrapper, "bandwidth_wrapper"},
    {817, sha_wrapper, "sha_wrapper"},
    {818, qsort_wrapper, "qsort_wrapper"},
    {819, mser_wrapper, "mser_wrapper"},
    {820, disparity_wrapper, "disparity_wrapper"},
    {821, fft_wrapper, "fft_wrapper"},
    {822, dijkstra_wrapper, "dijkstra_wrapper"},
    {823, sorting_wrapper, "sorting_wrapper"},
    // Scenario 104
    {824, disparity_wrapper, "disparity_wrapper"},
    {825, qsort_wrapper, "qsort_wrapper"},
    {826, fft_wrapper, "fft_wrapper"},
    {827, dijkstra_wrapper, "dijkstra_wrapper"},
    {828, bandwidth_wrapper, "bandwidth_wrapper"},
    {829, sorting_wrapper, "sorting_wrapper"},
    {830, sha_wrapper, "sha_wrapper"},
    {831, mser_wrapper, "mser_wrapper"},

    // ===========================================================================
    // ==  Category C: Targeted Stress & Monoculture Scenarios (Scenarios
    // 105-120) ==
    // ===========================================================================

    // --- Sub-Category: Full Monocultures (Scenarios 105-112) ---
    // Scenario 105: Full bandwidth
    {832, bandwidth_wrapper, "bandwidth_wrapper"},
    {833, bandwidth_wrapper, "bandwidth_wrapper"},
    {834, bandwidth_wrapper, "bandwidth_wrapper"},
    {835, bandwidth_wrapper, "bandwidth_wrapper"},
    {836, bandwidth_wrapper, "bandwidth_wrapper"},
    {837, bandwidth_wrapper, "bandwidth_wrapper"},
    {838, bandwidth_wrapper, "bandwidth_wrapper"},
    {839, bandwidth_wrapper, "bandwidth_wrapper"},
    // Scenario 106: Full disparity
    {840, disparity_wrapper, "disparity_wrapper"},
    {841, disparity_wrapper, "disparity_wrapper"},
    {842, disparity_wrapper, "disparity_wrapper"},
    {843, disparity_wrapper, "disparity_wrapper"},
    {844, disparity_wrapper, "disparity_wrapper"},
    {845, disparity_wrapper, "disparity_wrapper"},
    {846, disparity_wrapper, "disparity_wrapper"},
    {847, disparity_wrapper, "disparity_wrapper"},
    // Scenario 107: Full fft
    {848, fft_wrapper, "fft_wrapper"},
    {849, fft_wrapper, "fft_wrapper"},
    {850, fft_wrapper, "fft_wrapper"},
    {851, fft_wrapper, "fft_wrapper"},
    {852, fft_wrapper, "fft_wrapper"},
    {853, fft_wrapper, "fft_wrapper"},
    {854, fft_wrapper, "fft_wrapper"},
    {855, fft_wrapper, "fft_wrapper"},
    // Scenario 108: Full mser
    {856, mser_wrapper, "mser_wrapper"},
    {857, mser_wrapper, "mser_wrapper"},
    {858, mser_wrapper, "mser_wrapper"},
    {859, mser_wrapper, "mser_wrapper"},
    {860, mser_wrapper, "mser_wrapper"},
    {861, mser_wrapper, "mser_wrapper"},
    {862, mser_wrapper, "mser_wrapper"},
    {863, mser_wrapper, "mser_wrapper"},
    // Scenario 109: Full qsort
    {864, qsort_wrapper, "qsort_wrapper"},
    {865, qsort_wrapper, "qsort_wrapper"},
    {866, qsort_wrapper, "qsort_wrapper"},
    {867, qsort_wrapper, "qsort_wrapper"},
    {868, qsort_wrapper, "qsort_wrapper"},
    {869, qsort_wrapper, "qsort_wrapper"},
    {870, qsort_wrapper, "qsort_wrapper"},
    {871, qsort_wrapper, "qsort_wrapper"},
    // Scenario 110: Full sorting
    {872, sorting_wrapper, "sorting_wrapper"},
    {873, sorting_wrapper, "sorting_wrapper"},
    {874, sorting_wrapper, "sorting_wrapper"},
    {875, sorting_wrapper, "sorting_wrapper"},
    {876, sorting_wrapper, "sorting_wrapper"},
    {877, sorting_wrapper, "sorting_wrapper"},
    {878, sorting_wrapper, "sorting_wrapper"},
    {879, sorting_wrapper, "sorting_wrapper"},
    // Scenario 111: Full sha
    {880, sha_wrapper, "sha_wrapper"},
    {881, sha_wrapper, "sha_wrapper"},
    {882, sha_wrapper, "sha_wrapper"},
    {883, sha_wrapper, "sha_wrapper"},
    {884, sha_wrapper, "sha_wrapper"},
    {885, sha_wrapper, "sha_wrapper"},
    {886, sha_wrapper, "sha_wrapper"},
    {887, sha_wrapper, "sha_wrapper"},
    // Scenario 112: Full dijkstra
    {888, dijkstra_wrapper, "dijkstra_wrapper"},
    {889, dijkstra_wrapper, "dijkstra_wrapper"},
    {890, dijkstra_wrapper, "dijkstra_wrapper"},
    {891, dijkstra_wrapper, "dijkstra_wrapper"},
    {892, dijkstra_wrapper, "dijkstra_wrapper"},
    {893, dijkstra_wrapper, "dijkstra_wrapper"},
    {894, dijkstra_wrapper, "dijkstra_wrapper"},
    {895, dijkstra_wrapper, "dijkstra_wrapper"},

    // --- Sub-Category: Dual-Cultures (Scenarios 113-116) ---
    // Scenario 113: 4x bandwidth vs 4x fft
    {896, bandwidth_wrapper, "bandwidth_wrapper"},
    {897, bandwidth_wrapper, "bandwidth_wrapper"},
    {898, bandwidth_wrapper, "bandwidth_wrapper"},
    {899, bandwidth_wrapper, "bandwidth_wrapper"},
    {900, fft_wrapper, "fft_wrapper"},
    {901, fft_wrapper, "fft_wrapper"},
    {902, fft_wrapper, "fft_wrapper"},
    {903, fft_wrapper, "fft_wrapper"},
    // Scenario 114: 4x disparity vs 4x mser
    {904, disparity_wrapper, "disparity_wrapper"},
    {905, disparity_wrapper, "disparity_wrapper"},
    {906, disparity_wrapper, "disparity_wrapper"},
    {907, disparity_wrapper, "disparity_wrapper"},
    {908, mser_wrapper, "mser_wrapper"},
    {909, mser_wrapper, "mser_wrapper"},
    {910, mser_wrapper, "mser_wrapper"},
    {911, mser_wrapper, "mser_wrapper"},
    // Scenario 115: 4x bandwidth (H) vs 4x qsort (L)
    {912, bandwidth_wrapper, "bandwidth_wrapper"},
    {913, bandwidth_wrapper, "bandwidth_wrapper"},
    {914, bandwidth_wrapper, "bandwidth_wrapper"},
    {915, bandwidth_wrapper, "bandwidth_wrapper"},
    {916, qsort_wrapper, "qsort_wrapper"},
    {917, qsort_wrapper, "qsort_wrapper"},
    {918, qsort_wrapper, "qsort_wrapper"},
    {919, qsort_wrapper, "qsort_wrapper"},
    // Scenario 116: 4x fft (H) vs 4x sha (L)
    {920, fft_wrapper, "fft_wrapper"},
    {921, fft_wrapper, "fft_wrapper"},
    {922, fft_wrapper, "fft_wrapper"},
    {923, fft_wrapper, "fft_wrapper"},
    {924, sha_wrapper, "sha_wrapper"},
    {925, sha_wrapper, "sha_wrapper"},
    {926, sha_wrapper, "sha_wrapper"},
    {927, sha_wrapper, "sha_wrapper"},

    // --- Sub-Category: Inversion Tests (Scenarios 117-120) ---
    // Scenario 117: vm0 (L,L) vs 3 VMs bandwidth mono
    {928, qsort_wrapper, "qsort_wrapper"},
    {929, sorting_wrapper, "sorting_wrapper"},
    {930, bandwidth_wrapper, "bandwidth_wrapper"},
    {931, bandwidth_wrapper, "bandwidth_wrapper"},
    {932, bandwidth_wrapper, "bandwidth_wrapper"},
    {933, bandwidth_wrapper, "bandwidth_wrapper"},
    {934, bandwidth_wrapper, "bandwidth_wrapper"},
    {935, bandwidth_wrapper, "bandwidth_wrapper"},
    // Scenario 118: vm0 (L,L) vs 3 VMs fft mono
    {936, sha_wrapper, "sha_wrapper"},
    {937, dijkstra_wrapper, "dijkstra_wrapper"},
    {938, fft_wrapper, "fft_wrapper"},
    {939, fft_wrapper, "fft_wrapper"},
    {940, fft_wrapper, "fft_wrapper"},
    {941, fft_wrapper, "fft_wrapper"},
    {942, fft_wrapper, "fft_wrapper"},
    {943, fft_wrapper, "fft_wrapper"},
    // Scenario 119: vm0 (L,L) vs 3 VMs disparity mono
    {944, qsort_wrapper, "qsort_wrapper"},
    {945, sha_wrapper, "sha_wrapper"},
    {946, disparity_wrapper, "disparity_wrapper"},
    {947, disparity_wrapper, "disparity_wrapper"},
    {948, disparity_wrapper, "disparity_wrapper"},
    {949, disparity_wrapper, "disparity_wrapper"},
    {950, disparity_wrapper, "disparity_wrapper"},
    {951, disparity_wrapper, "disparity_wrapper"},
    // Scenario 120: vm0 (L,L) vs 3 VMs mser mono
    {952, sorting_wrapper, "sorting_wrapper"},
    {953, dijkstra_wrapper, "dijkstra_wrapper"},
    {954, mser_wrapper, "mser_wrapper"},
    {955, mser_wrapper, "mser_wrapper"},
    {956, mser_wrapper, "mser_wrapper"},
    {957, mser_wrapper, "mser_wrapper"},
    {958, mser_wrapper, "mser_wrapper"},
    {959, mser_wrapper, "mser_wrapper"},
};

BenchInfo benchmark_info[NUM_BENCHMARKS];
formula_t budget_formula = 0;

////////

int get_benchmark_index(int vm_num, int task_num) {
  int index =
      (vm_num * TASK_QUANTITY + task_num) + (MAX_TASKS * BENCH_ARRAY_INDEX);
  if (index < 0 || index >= NUM_BENCHMARKS) {
    printf("Invalid get_benchmark_index call %d\n", index);
    return -1;
  }
  return index;
}

void set_budget_formula(formula_t formula) {
  if (formula < 0 || formula >= FORMULA_COUNT) {
    printf("Invalid set_benchmark_formula call %d\n", formula);
    return;
  }
  budget_formula = formula;
}

formula_t get_budget_formula() { return budget_formula; }

BenchInfo *get_benchmark_info(int vm_num, int task_num) {
  if (!init) {
    printf("not initialized\n");
    return NULL;
  }
  //
  int index = get_benchmark_index(vm_num, task_num);
  BenchInfo *benchInfo = &benchmark_info[index];
  if (benchInfo == NULL) {
    printf("NULL benchInfo\n");
  }
  return benchInfo;
}

BenchInfo *add_benchmark_info(int vm_num, int task_num, int periodicity) {
  BenchInfo info;
  int index = get_benchmark_index(vm_num, task_num);
  info.function = benchmark_functions[index];
  info.task_num = task_num;
  // info.task_handle = handler;
  // info.budget_formula = AFC_FORMULA;
  info.periodicity = periodicity;
  info.task_overruns = 0;
  info.task_underruns = 0;

  benchmark_info[index] = info;
  return &benchmark_info[index];
}

void init_bench() {
  init_data();

  for (int index = 0; index < NUM_BENCHMARKS; ++index) {
    benchmark_info[index].function.index = -1;
    benchmark_info[index].function.name = "UNDEFINED";
    benchmark_info[index].function.pointer = empty_wrapper;
    benchmark_info[index].task_num = -1;
    benchmark_info[index].periodicity = 0;
    benchmark_info[index].task_overruns = -1;
    benchmark_info[index].task_underruns = -1;
  }

  init = true;
}

void destroy_bench() {
  if (init) {
    free_data();
  }
}
