
#include "../inc/benchmarks.h"

// #-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#
#include "../inc/regulation.h"

#include <stdbool.h>

// Global variables for all benchmark functions
#define NUM_BENCHMARKS 1280
#define MAX_TASKS 8

// Generating scenarios...
// Generated 480 chaotic mix scenarios.
// Generation complete.
Function benchmark_functions[NUM_BENCHMARKS] = {
    // Scenario 1
    {0, bandwidth_wrapper, "bandwidth_wrapper"},
    {1, bandwidth_wrapper, "bandwidth_wrapper"},
    {2, dijkstra_wrapper, "dijkstra_wrapper"},
    {3, bandwidth_wrapper, "bandwidth_wrapper"},
    {4, bandwidth_wrapper, "bandwidth_wrapper"},
    {5, disparity_wrapper, "disparity_wrapper"},
    {6, sha_wrapper, "sha_wrapper"},
    {7, mser_wrapper, "mser_wrapper"},
    // Scenario 2
    {8, bandwidth_wrapper, "bandwidth_wrapper"},
    {9, bandwidth_wrapper, "bandwidth_wrapper"},
    {10, dijkstra_wrapper, "dijkstra_wrapper"},
    {11, bandwidth_wrapper, "bandwidth_wrapper"},
    {12, bandwidth_wrapper, "bandwidth_wrapper"},
    {13, fft_wrapper, "fft_wrapper"},
    {14, dijkstra_wrapper, "dijkstra_wrapper"},
    {15, fft_wrapper, "fft_wrapper"},
    // Scenario 3
    {16, bandwidth_wrapper, "bandwidth_wrapper"},
    {17, bandwidth_wrapper, "bandwidth_wrapper"},
    {18, dijkstra_wrapper, "dijkstra_wrapper"},
    {19, mser_wrapper, "mser_wrapper"},
    {20, disparity_wrapper, "disparity_wrapper"},
    {21, mser_wrapper, "mser_wrapper"},
    {22, disparity_wrapper, "disparity_wrapper"},
    {23, disparity_wrapper, "disparity_wrapper"},
    // Scenario 4
    {24, bandwidth_wrapper, "bandwidth_wrapper"},
    {25, bandwidth_wrapper, "bandwidth_wrapper"},
    {26, dijkstra_wrapper, "dijkstra_wrapper"},
    {27, sorting_wrapper, "sorting_wrapper"},
    {28, mser_wrapper, "mser_wrapper"},
    {29, fft_wrapper, "fft_wrapper"},
    {30, fft_wrapper, "fft_wrapper"},
    {31, fft_wrapper, "fft_wrapper"},
    // Scenario 5
    {32, bandwidth_wrapper, "bandwidth_wrapper"},
    {33, bandwidth_wrapper, "bandwidth_wrapper"},
    {34, fft_wrapper, "fft_wrapper"},
    {35, dijkstra_wrapper, "dijkstra_wrapper"},
    {36, sorting_wrapper, "sorting_wrapper"},
    {37, disparity_wrapper, "disparity_wrapper"},
    {38, fft_wrapper, "fft_wrapper"},
    {39, disparity_wrapper, "disparity_wrapper"},
    // Scenario 6
    {40, bandwidth_wrapper, "bandwidth_wrapper"},
    {41, bandwidth_wrapper, "bandwidth_wrapper"},
    {42, mser_wrapper, "mser_wrapper"},
    {43, sha_wrapper, "sha_wrapper"},
    {44, fft_wrapper, "fft_wrapper"},
    {45, mser_wrapper, "mser_wrapper"},
    {46, fft_wrapper, "fft_wrapper"},
    {47, fft_wrapper, "fft_wrapper"},
    // Scenario 7
    {48, bandwidth_wrapper, "bandwidth_wrapper"},
    {49, bandwidth_wrapper, "bandwidth_wrapper"},
    {50, mser_wrapper, "mser_wrapper"},
    {51, sha_wrapper, "sha_wrapper"},
    {52, sha_wrapper, "sha_wrapper"},
    {53, sha_wrapper, "sha_wrapper"},
    {54, disparity_wrapper, "disparity_wrapper"},
    {55, disparity_wrapper, "disparity_wrapper"},
    // Scenario 8
    {56, bandwidth_wrapper, "bandwidth_wrapper"},
    {57, bandwidth_wrapper, "bandwidth_wrapper"},
    {58, mser_wrapper, "mser_wrapper"},
    {59, sha_wrapper, "sha_wrapper"},
    {60, sorting_wrapper, "sorting_wrapper"},
    {61, sorting_wrapper, "sorting_wrapper"},
    {62, sorting_wrapper, "sorting_wrapper"},
    {63, sorting_wrapper, "sorting_wrapper"},
    // Scenario 9
    {64, bandwidth_wrapper, "bandwidth_wrapper"},
    {65, bandwidth_wrapper, "bandwidth_wrapper"},
    {66, sorting_wrapper, "sorting_wrapper"},
    {67, bandwidth_wrapper, "bandwidth_wrapper"},
    {68, mser_wrapper, "mser_wrapper"},
    {69, mser_wrapper, "mser_wrapper"},
    {70, sha_wrapper, "sha_wrapper"},
    {71, mser_wrapper, "mser_wrapper"},
    // Scenario 10
    {72, bandwidth_wrapper, "bandwidth_wrapper"},
    {73, bandwidth_wrapper, "bandwidth_wrapper"},
    {74, sorting_wrapper, "sorting_wrapper"},
    {75, fft_wrapper, "fft_wrapper"},
    {76, fft_wrapper, "fft_wrapper"},
    {77, dijkstra_wrapper, "dijkstra_wrapper"},
    {78, disparity_wrapper, "disparity_wrapper"},
    {79, sorting_wrapper, "sorting_wrapper"},
    // Scenario 11
    {80, bandwidth_wrapper, "bandwidth_wrapper"},
    {81, dijkstra_wrapper, "dijkstra_wrapper"},
    {82, bandwidth_wrapper, "bandwidth_wrapper"},
    {83, sorting_wrapper, "sorting_wrapper"},
    {84, mser_wrapper, "mser_wrapper"},
    {85, disparity_wrapper, "disparity_wrapper"},
    {86, sorting_wrapper, "sorting_wrapper"},
    {87, sorting_wrapper, "sorting_wrapper"},
    // Scenario 12
    {88, bandwidth_wrapper, "bandwidth_wrapper"},
    {89, dijkstra_wrapper, "dijkstra_wrapper"},
    {90, disparity_wrapper, "disparity_wrapper"},
    {91, mser_wrapper, "mser_wrapper"},
    {92, disparity_wrapper, "disparity_wrapper"},
    {93, dijkstra_wrapper, "dijkstra_wrapper"},
    {94, dijkstra_wrapper, "dijkstra_wrapper"},
    {95, bandwidth_wrapper, "bandwidth_wrapper"},
    // Scenario 13
    {96, bandwidth_wrapper, "bandwidth_wrapper"},
    {97, dijkstra_wrapper, "dijkstra_wrapper"},
    {98, mser_wrapper, "mser_wrapper"},
    {99, bandwidth_wrapper, "bandwidth_wrapper"},
    {100, sorting_wrapper, "sorting_wrapper"},
    {101, mser_wrapper, "mser_wrapper"},
    {102, mser_wrapper, "mser_wrapper"},
    {103, mser_wrapper, "mser_wrapper"},
    // Scenario 14
    {104, bandwidth_wrapper, "bandwidth_wrapper"},
    {105, dijkstra_wrapper, "dijkstra_wrapper"},
    {106, sha_wrapper, "sha_wrapper"},
    {107, disparity_wrapper, "disparity_wrapper"},
    {108, sha_wrapper, "sha_wrapper"},
    {109, bandwidth_wrapper, "bandwidth_wrapper"},
    {110, mser_wrapper, "mser_wrapper"},
    {111, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 15
    {112, bandwidth_wrapper, "bandwidth_wrapper"},
    {113, dijkstra_wrapper, "dijkstra_wrapper"},
    {114, sha_wrapper, "sha_wrapper"},
    {115, sha_wrapper, "sha_wrapper"},
    {116, sorting_wrapper, "sorting_wrapper"},
    {117, dijkstra_wrapper, "dijkstra_wrapper"},
    {118, fft_wrapper, "fft_wrapper"},
    {119, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 16
    {120, bandwidth_wrapper, "bandwidth_wrapper"},
    {121, disparity_wrapper, "disparity_wrapper"},
    {122, dijkstra_wrapper, "dijkstra_wrapper"},
    {123, fft_wrapper, "fft_wrapper"},
    {124, bandwidth_wrapper, "bandwidth_wrapper"},
    {125, disparity_wrapper, "disparity_wrapper"},
    {126, sha_wrapper, "sha_wrapper"},
    {127, disparity_wrapper, "disparity_wrapper"},
    // Scenario 17
    {128, bandwidth_wrapper, "bandwidth_wrapper"},
    {129, disparity_wrapper, "disparity_wrapper"},
    {130, dijkstra_wrapper, "dijkstra_wrapper"},
    {131, sha_wrapper, "sha_wrapper"},
    {132, mser_wrapper, "mser_wrapper"},
    {133, fft_wrapper, "fft_wrapper"},
    {134, sha_wrapper, "sha_wrapper"},
    {135, mser_wrapper, "mser_wrapper"},
    // Scenario 18
    {136, bandwidth_wrapper, "bandwidth_wrapper"},
    {137, disparity_wrapper, "disparity_wrapper"},
    {138, fft_wrapper, "fft_wrapper"},
    {139, fft_wrapper, "fft_wrapper"},
    {140, disparity_wrapper, "disparity_wrapper"},
    {141, sorting_wrapper, "sorting_wrapper"},
    {142, mser_wrapper, "mser_wrapper"},
    {143, sorting_wrapper, "sorting_wrapper"},
    // Scenario 19
    {144, bandwidth_wrapper, "bandwidth_wrapper"},
    {145, disparity_wrapper, "disparity_wrapper"},
    {146, mser_wrapper, "mser_wrapper"},
    {147, bandwidth_wrapper, "bandwidth_wrapper"},
    {148, disparity_wrapper, "disparity_wrapper"},
    {149, sha_wrapper, "sha_wrapper"},
    {150, sha_wrapper, "sha_wrapper"},
    {151, fft_wrapper, "fft_wrapper"},
    // Scenario 20
    {152, bandwidth_wrapper, "bandwidth_wrapper"},
    {153, disparity_wrapper, "disparity_wrapper"},
    {154, mser_wrapper, "mser_wrapper"},
    {155, bandwidth_wrapper, "bandwidth_wrapper"},
    {156, fft_wrapper, "fft_wrapper"},
    {157, bandwidth_wrapper, "bandwidth_wrapper"},
    {158, sorting_wrapper, "sorting_wrapper"},
    {159, fft_wrapper, "fft_wrapper"},
    // Scenario 21
    {160, bandwidth_wrapper, "bandwidth_wrapper"},
    {161, disparity_wrapper, "disparity_wrapper"},
    {162, sha_wrapper, "sha_wrapper"},
    {163, disparity_wrapper, "disparity_wrapper"},
    {164, disparity_wrapper, "disparity_wrapper"},
    {165, bandwidth_wrapper, "bandwidth_wrapper"},
    {166, sha_wrapper, "sha_wrapper"},
    {167, bandwidth_wrapper, "bandwidth_wrapper"},
    // Scenario 22
    {168, bandwidth_wrapper, "bandwidth_wrapper"},
    {169, disparity_wrapper, "disparity_wrapper"},
    {170, sha_wrapper, "sha_wrapper"},
    {171, fft_wrapper, "fft_wrapper"},
    {172, dijkstra_wrapper, "dijkstra_wrapper"},
    {173, dijkstra_wrapper, "dijkstra_wrapper"},
    {174, bandwidth_wrapper, "bandwidth_wrapper"},
    {175, disparity_wrapper, "disparity_wrapper"},
    // Scenario 23
    {176, bandwidth_wrapper, "bandwidth_wrapper"},
    {177, disparity_wrapper, "disparity_wrapper"},
    {178, sha_wrapper, "sha_wrapper"},
    {179, mser_wrapper, "mser_wrapper"},
    {180, dijkstra_wrapper, "dijkstra_wrapper"},
    {181, dijkstra_wrapper, "dijkstra_wrapper"},
    {182, mser_wrapper, "mser_wrapper"},
    {183, sorting_wrapper, "sorting_wrapper"},
    // Scenario 24
    {184, bandwidth_wrapper, "bandwidth_wrapper"},
    {185, disparity_wrapper, "disparity_wrapper"},
    {186, sorting_wrapper, "sorting_wrapper"},
    {187, bandwidth_wrapper, "bandwidth_wrapper"},
    {188, dijkstra_wrapper, "dijkstra_wrapper"},
    {189, bandwidth_wrapper, "bandwidth_wrapper"},
    {190, dijkstra_wrapper, "dijkstra_wrapper"},
    {191, sorting_wrapper, "sorting_wrapper"},
    // Scenario 25
    {192, bandwidth_wrapper, "bandwidth_wrapper"},
    {193, disparity_wrapper, "disparity_wrapper"},
    {194, sorting_wrapper, "sorting_wrapper"},
    {195, dijkstra_wrapper, "dijkstra_wrapper"},
    {196, sha_wrapper, "sha_wrapper"},
    {197, bandwidth_wrapper, "bandwidth_wrapper"},
    {198, fft_wrapper, "fft_wrapper"},
    {199, bandwidth_wrapper, "bandwidth_wrapper"},
    // Scenario 26
    {200, bandwidth_wrapper, "bandwidth_wrapper"},
    {201, fft_wrapper, "fft_wrapper"},
    {202, bandwidth_wrapper, "bandwidth_wrapper"},
    {203, sha_wrapper, "sha_wrapper"},
    {204, sorting_wrapper, "sorting_wrapper"},
    {205, fft_wrapper, "fft_wrapper"},
    {206, disparity_wrapper, "disparity_wrapper"},
    {207, sorting_wrapper, "sorting_wrapper"},
    // Scenario 27
    {208, bandwidth_wrapper, "bandwidth_wrapper"},
    {209, fft_wrapper, "fft_wrapper"},
    {210, bandwidth_wrapper, "bandwidth_wrapper"},
    {211, sha_wrapper, "sha_wrapper"},
    {212, sorting_wrapper, "sorting_wrapper"},
    {213, mser_wrapper, "mser_wrapper"},
    {214, dijkstra_wrapper, "dijkstra_wrapper"},
    {215, sha_wrapper, "sha_wrapper"},
    // Scenario 28
    {216, bandwidth_wrapper, "bandwidth_wrapper"},
    {217, fft_wrapper, "fft_wrapper"},
    {218, dijkstra_wrapper, "dijkstra_wrapper"},
    {219, dijkstra_wrapper, "dijkstra_wrapper"},
    {220, sha_wrapper, "sha_wrapper"},
    {221, disparity_wrapper, "disparity_wrapper"},
    {222, disparity_wrapper, "disparity_wrapper"},
    {223, mser_wrapper, "mser_wrapper"},
    // Scenario 29
    {224, bandwidth_wrapper, "bandwidth_wrapper"},
    {225, fft_wrapper, "fft_wrapper"},
    {226, dijkstra_wrapper, "dijkstra_wrapper"},
    {227, dijkstra_wrapper, "dijkstra_wrapper"},
    {228, sha_wrapper, "sha_wrapper"},
    {229, mser_wrapper, "mser_wrapper"},
    {230, bandwidth_wrapper, "bandwidth_wrapper"},
    {231, disparity_wrapper, "disparity_wrapper"},
    // Scenario 30
    {232, bandwidth_wrapper, "bandwidth_wrapper"},
    {233, fft_wrapper, "fft_wrapper"},
    {234, fft_wrapper, "fft_wrapper"},
    {235, disparity_wrapper, "disparity_wrapper"},
    {236, dijkstra_wrapper, "dijkstra_wrapper"},
    {237, bandwidth_wrapper, "bandwidth_wrapper"},
    {238, sha_wrapper, "sha_wrapper"},
    {239, fft_wrapper, "fft_wrapper"},
    // Scenario 31
    {240, bandwidth_wrapper, "bandwidth_wrapper"},
    {241, fft_wrapper, "fft_wrapper"},
    {242, fft_wrapper, "fft_wrapper"},
    {243, mser_wrapper, "mser_wrapper"},
    {244, mser_wrapper, "mser_wrapper"},
    {245, bandwidth_wrapper, "bandwidth_wrapper"},
    {246, disparity_wrapper, "disparity_wrapper"},
    {247, mser_wrapper, "mser_wrapper"},
    // Scenario 32
    {248, bandwidth_wrapper, "bandwidth_wrapper"},
    {249, fft_wrapper, "fft_wrapper"},
    {250, sha_wrapper, "sha_wrapper"},
    {251, disparity_wrapper, "disparity_wrapper"},
    {252, sha_wrapper, "sha_wrapper"},
    {253, disparity_wrapper, "disparity_wrapper"},
    {254, sha_wrapper, "sha_wrapper"},
    {255, disparity_wrapper, "disparity_wrapper"},
    // Scenario 33
    {256, bandwidth_wrapper, "bandwidth_wrapper"},
    {257, fft_wrapper, "fft_wrapper"},
    {258, sorting_wrapper, "sorting_wrapper"},
    {259, bandwidth_wrapper, "bandwidth_wrapper"},
    {260, sorting_wrapper, "sorting_wrapper"},
    {261, disparity_wrapper, "disparity_wrapper"},
    {262, mser_wrapper, "mser_wrapper"},
    {263, bandwidth_wrapper, "bandwidth_wrapper"},
    // Scenario 34
    {264, bandwidth_wrapper, "bandwidth_wrapper"},
    {265, mser_wrapper, "mser_wrapper"},
    {266, disparity_wrapper, "disparity_wrapper"},
    {267, dijkstra_wrapper, "dijkstra_wrapper"},
    {268, sorting_wrapper, "sorting_wrapper"},
    {269, mser_wrapper, "mser_wrapper"},
    {270, fft_wrapper, "fft_wrapper"},
    {271, fft_wrapper, "fft_wrapper"},
    // Scenario 35
    {272, bandwidth_wrapper, "bandwidth_wrapper"},
    {273, mser_wrapper, "mser_wrapper"},
    {274, disparity_wrapper, "disparity_wrapper"},
    {275, disparity_wrapper, "disparity_wrapper"},
    {276, dijkstra_wrapper, "dijkstra_wrapper"},
    {277, bandwidth_wrapper, "bandwidth_wrapper"},
    {278, sorting_wrapper, "sorting_wrapper"},
    {279, disparity_wrapper, "disparity_wrapper"},
    // Scenario 36
    {280, bandwidth_wrapper, "bandwidth_wrapper"},
    {281, mser_wrapper, "mser_wrapper"},
    {282, disparity_wrapper, "disparity_wrapper"},
    {283, fft_wrapper, "fft_wrapper"},
    {284, sha_wrapper, "sha_wrapper"},
    {285, sha_wrapper, "sha_wrapper"},
    {286, disparity_wrapper, "disparity_wrapper"},
    {287, sha_wrapper, "sha_wrapper"},
    // Scenario 37
    {288, bandwidth_wrapper, "bandwidth_wrapper"},
    {289, mser_wrapper, "mser_wrapper"},
    {290, disparity_wrapper, "disparity_wrapper"},
    {291, mser_wrapper, "mser_wrapper"},
    {292, fft_wrapper, "fft_wrapper"},
    {293, sha_wrapper, "sha_wrapper"},
    {294, dijkstra_wrapper, "dijkstra_wrapper"},
    {295, sorting_wrapper, "sorting_wrapper"},
    // Scenario 38
    {296, bandwidth_wrapper, "bandwidth_wrapper"},
    {297, mser_wrapper, "mser_wrapper"},
    {298, fft_wrapper, "fft_wrapper"},
    {299, disparity_wrapper, "disparity_wrapper"},
    {300, fft_wrapper, "fft_wrapper"},
    {301, mser_wrapper, "mser_wrapper"},
    {302, disparity_wrapper, "disparity_wrapper"},
    {303, bandwidth_wrapper, "bandwidth_wrapper"},
    // Scenario 39
    {304, bandwidth_wrapper, "bandwidth_wrapper"},
    {305, mser_wrapper, "mser_wrapper"},
    {306, fft_wrapper, "fft_wrapper"},
    {307, disparity_wrapper, "disparity_wrapper"},
    {308, sorting_wrapper, "sorting_wrapper"},
    {309, disparity_wrapper, "disparity_wrapper"},
    {310, disparity_wrapper, "disparity_wrapper"},
    {311, sha_wrapper, "sha_wrapper"},
    // Scenario 40
    {312, bandwidth_wrapper, "bandwidth_wrapper"},
    {313, mser_wrapper, "mser_wrapper"},
    {314, fft_wrapper, "fft_wrapper"},
    {315, sha_wrapper, "sha_wrapper"},
    {316, fft_wrapper, "fft_wrapper"},
    {317, dijkstra_wrapper, "dijkstra_wrapper"},
    {318, bandwidth_wrapper, "bandwidth_wrapper"},
    {319, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 41
    {320, bandwidth_wrapper, "bandwidth_wrapper"},
    {321, mser_wrapper, "mser_wrapper"},
    {322, sha_wrapper, "sha_wrapper"},
    {323, sha_wrapper, "sha_wrapper"},
    {324, disparity_wrapper, "disparity_wrapper"},
    {325, bandwidth_wrapper, "bandwidth_wrapper"},
    {326, sorting_wrapper, "sorting_wrapper"},
    {327, bandwidth_wrapper, "bandwidth_wrapper"},
    // Scenario 42
    {328, bandwidth_wrapper, "bandwidth_wrapper"},
    {329, mser_wrapper, "mser_wrapper"},
    {330, sorting_wrapper, "sorting_wrapper"},
    {331, sha_wrapper, "sha_wrapper"},
    {332, mser_wrapper, "mser_wrapper"},
    {333, fft_wrapper, "fft_wrapper"},
    {334, disparity_wrapper, "disparity_wrapper"},
    {335, disparity_wrapper, "disparity_wrapper"},
    // Scenario 43
    {336, bandwidth_wrapper, "bandwidth_wrapper"},
    {337, mser_wrapper, "mser_wrapper"},
    {338, sorting_wrapper, "sorting_wrapper"},
    {339, sorting_wrapper, "sorting_wrapper"},
    {340, disparity_wrapper, "disparity_wrapper"},
    {341, sha_wrapper, "sha_wrapper"},
    {342, disparity_wrapper, "disparity_wrapper"},
    {343, sha_wrapper, "sha_wrapper"},
    // Scenario 44
    {344, bandwidth_wrapper, "bandwidth_wrapper"},
    {345, sha_wrapper, "sha_wrapper"},
    {346, bandwidth_wrapper, "bandwidth_wrapper"},
    {347, bandwidth_wrapper, "bandwidth_wrapper"},
    {348, disparity_wrapper, "disparity_wrapper"},
    {349, dijkstra_wrapper, "dijkstra_wrapper"},
    {350, bandwidth_wrapper, "bandwidth_wrapper"},
    {351, sorting_wrapper, "sorting_wrapper"},
    // Scenario 45
    {352, bandwidth_wrapper, "bandwidth_wrapper"},
    {353, sha_wrapper, "sha_wrapper"},
    {354, bandwidth_wrapper, "bandwidth_wrapper"},
    {355, sha_wrapper, "sha_wrapper"},
    {356, disparity_wrapper, "disparity_wrapper"},
    {357, sha_wrapper, "sha_wrapper"},
    {358, bandwidth_wrapper, "bandwidth_wrapper"},
    {359, sha_wrapper, "sha_wrapper"},
    // Scenario 46
    {360, bandwidth_wrapper, "bandwidth_wrapper"},
    {361, sha_wrapper, "sha_wrapper"},
    {362, bandwidth_wrapper, "bandwidth_wrapper"},
    {363, sha_wrapper, "sha_wrapper"},
    {364, sorting_wrapper, "sorting_wrapper"},
    {365, fft_wrapper, "fft_wrapper"},
    {366, fft_wrapper, "fft_wrapper"},
    {367, disparity_wrapper, "disparity_wrapper"},
    // Scenario 47
    {368, bandwidth_wrapper, "bandwidth_wrapper"},
    {369, sha_wrapper, "sha_wrapper"},
    {370, dijkstra_wrapper, "dijkstra_wrapper"},
    {371, fft_wrapper, "fft_wrapper"},
    {372, dijkstra_wrapper, "dijkstra_wrapper"},
    {373, dijkstra_wrapper, "dijkstra_wrapper"},
    {374, mser_wrapper, "mser_wrapper"},
    {375, sha_wrapper, "sha_wrapper"},
    // Scenario 48
    {376, bandwidth_wrapper, "bandwidth_wrapper"},
    {377, sha_wrapper, "sha_wrapper"},
    {378, dijkstra_wrapper, "dijkstra_wrapper"},
    {379, fft_wrapper, "fft_wrapper"},
    {380, dijkstra_wrapper, "dijkstra_wrapper"},
    {381, fft_wrapper, "fft_wrapper"},
    {382, sorting_wrapper, "sorting_wrapper"},
    {383, mser_wrapper, "mser_wrapper"},
    // Scenario 49
    {384, bandwidth_wrapper, "bandwidth_wrapper"},
    {385, sha_wrapper, "sha_wrapper"},
    {386, disparity_wrapper, "disparity_wrapper"},
    {387, bandwidth_wrapper, "bandwidth_wrapper"},
    {388, dijkstra_wrapper, "dijkstra_wrapper"},
    {389, disparity_wrapper, "disparity_wrapper"},
    {390, bandwidth_wrapper, "bandwidth_wrapper"},
    {391, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 50
    {392, bandwidth_wrapper, "bandwidth_wrapper"},
    {393, sha_wrapper, "sha_wrapper"},
    {394, disparity_wrapper, "disparity_wrapper"},
    {395, dijkstra_wrapper, "dijkstra_wrapper"},
    {396, fft_wrapper, "fft_wrapper"},
    {397, disparity_wrapper, "disparity_wrapper"},
    {398, fft_wrapper, "fft_wrapper"},
    {399, sorting_wrapper, "sorting_wrapper"},
    // Scenario 51
    {400, bandwidth_wrapper, "bandwidth_wrapper"},
    {401, sha_wrapper, "sha_wrapper"},
    {402, disparity_wrapper, "disparity_wrapper"},
    {403, fft_wrapper, "fft_wrapper"},
    {404, sorting_wrapper, "sorting_wrapper"},
    {405, mser_wrapper, "mser_wrapper"},
    {406, fft_wrapper, "fft_wrapper"},
    {407, bandwidth_wrapper, "bandwidth_wrapper"},
    // Scenario 52
    {408, bandwidth_wrapper, "bandwidth_wrapper"},
    {409, sha_wrapper, "sha_wrapper"},
    {410, disparity_wrapper, "disparity_wrapper"},
    {411, sorting_wrapper, "sorting_wrapper"},
    {412, fft_wrapper, "fft_wrapper"},
    {413, mser_wrapper, "mser_wrapper"},
    {414, mser_wrapper, "mser_wrapper"},
    {415, sorting_wrapper, "sorting_wrapper"},
    // Scenario 53
    {416, bandwidth_wrapper, "bandwidth_wrapper"},
    {417, sha_wrapper, "sha_wrapper"},
    {418, disparity_wrapper, "disparity_wrapper"},
    {419, sorting_wrapper, "sorting_wrapper"},
    {420, sha_wrapper, "sha_wrapper"},
    {421, fft_wrapper, "fft_wrapper"},
    {422, fft_wrapper, "fft_wrapper"},
    {423, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 54
    {424, bandwidth_wrapper, "bandwidth_wrapper"},
    {425, sha_wrapper, "sha_wrapper"},
    {426, mser_wrapper, "mser_wrapper"},
    {427, mser_wrapper, "mser_wrapper"},
    {428, disparity_wrapper, "disparity_wrapper"},
    {429, fft_wrapper, "fft_wrapper"},
    {430, dijkstra_wrapper, "dijkstra_wrapper"},
    {431, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 55
    {432, bandwidth_wrapper, "bandwidth_wrapper"},
    {433, sha_wrapper, "sha_wrapper"},
    {434, mser_wrapper, "mser_wrapper"},
    {435, mser_wrapper, "mser_wrapper"},
    {436, sorting_wrapper, "sorting_wrapper"},
    {437, bandwidth_wrapper, "bandwidth_wrapper"},
    {438, mser_wrapper, "mser_wrapper"},
    {439, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 56
    {440, bandwidth_wrapper, "bandwidth_wrapper"},
    {441, sha_wrapper, "sha_wrapper"},
    {442, mser_wrapper, "mser_wrapper"},
    {443, sorting_wrapper, "sorting_wrapper"},
    {444, sorting_wrapper, "sorting_wrapper"},
    {445, sha_wrapper, "sha_wrapper"},
    {446, dijkstra_wrapper, "dijkstra_wrapper"},
    {447, sha_wrapper, "sha_wrapper"},
    // Scenario 57
    {448, bandwidth_wrapper, "bandwidth_wrapper"},
    {449, sha_wrapper, "sha_wrapper"},
    {450, sha_wrapper, "sha_wrapper"},
    {451, sha_wrapper, "sha_wrapper"},
    {452, disparity_wrapper, "disparity_wrapper"},
    {453, sorting_wrapper, "sorting_wrapper"},
    {454, dijkstra_wrapper, "dijkstra_wrapper"},
    {455, disparity_wrapper, "disparity_wrapper"},
    // Scenario 58
    {456, bandwidth_wrapper, "bandwidth_wrapper"},
    {457, sorting_wrapper, "sorting_wrapper"},
    {458, dijkstra_wrapper, "dijkstra_wrapper"},
    {459, dijkstra_wrapper, "dijkstra_wrapper"},
    {460, disparity_wrapper, "disparity_wrapper"},
    {461, disparity_wrapper, "disparity_wrapper"},
    {462, disparity_wrapper, "disparity_wrapper"},
    {463, sha_wrapper, "sha_wrapper"},
    // Scenario 59
    {464, bandwidth_wrapper, "bandwidth_wrapper"},
    {465, sorting_wrapper, "sorting_wrapper"},
    {466, disparity_wrapper, "disparity_wrapper"},
    {467, fft_wrapper, "fft_wrapper"},
    {468, mser_wrapper, "mser_wrapper"},
    {469, dijkstra_wrapper, "dijkstra_wrapper"},
    {470, fft_wrapper, "fft_wrapper"},
    {471, disparity_wrapper, "disparity_wrapper"},
    // Scenario 60
    {472, bandwidth_wrapper, "bandwidth_wrapper"},
    {473, sorting_wrapper, "sorting_wrapper"},
    {474, disparity_wrapper, "disparity_wrapper"},
    {475, mser_wrapper, "mser_wrapper"},
    {476, bandwidth_wrapper, "bandwidth_wrapper"},
    {477, bandwidth_wrapper, "bandwidth_wrapper"},
    {478, sha_wrapper, "sha_wrapper"},
    {479, fft_wrapper, "fft_wrapper"},
    // Scenario 61
    {480, bandwidth_wrapper, "bandwidth_wrapper"},
    {481, sorting_wrapper, "sorting_wrapper"},
    {482, disparity_wrapper, "disparity_wrapper"},
    {483, sha_wrapper, "sha_wrapper"},
    {484, fft_wrapper, "fft_wrapper"},
    {485, mser_wrapper, "mser_wrapper"},
    {486, bandwidth_wrapper, "bandwidth_wrapper"},
    {487, mser_wrapper, "mser_wrapper"},
    // Scenario 62
    {488, bandwidth_wrapper, "bandwidth_wrapper"},
    {489, sorting_wrapper, "sorting_wrapper"},
    {490, fft_wrapper, "fft_wrapper"},
    {491, bandwidth_wrapper, "bandwidth_wrapper"},
    {492, fft_wrapper, "fft_wrapper"},
    {493, mser_wrapper, "mser_wrapper"},
    {494, disparity_wrapper, "disparity_wrapper"},
    {495, sorting_wrapper, "sorting_wrapper"},
    // Scenario 63
    {496, bandwidth_wrapper, "bandwidth_wrapper"},
    {497, sorting_wrapper, "sorting_wrapper"},
    {498, mser_wrapper, "mser_wrapper"},
    {499, fft_wrapper, "fft_wrapper"},
    {500, bandwidth_wrapper, "bandwidth_wrapper"},
    {501, disparity_wrapper, "disparity_wrapper"},
    {502, fft_wrapper, "fft_wrapper"},
    {503, sorting_wrapper, "sorting_wrapper"},
    // Scenario 64
    {504, bandwidth_wrapper, "bandwidth_wrapper"},
    {505, sorting_wrapper, "sorting_wrapper"},
    {506, sha_wrapper, "sha_wrapper"},
    {507, dijkstra_wrapper, "dijkstra_wrapper"},
    {508, fft_wrapper, "fft_wrapper"},
    {509, sorting_wrapper, "sorting_wrapper"},
    {510, sha_wrapper, "sha_wrapper"},
    {511, fft_wrapper, "fft_wrapper"},
    // Scenario 65
    {512, bandwidth_wrapper, "bandwidth_wrapper"},
    {513, sorting_wrapper, "sorting_wrapper"},
    {514, sha_wrapper, "sha_wrapper"},
    {515, disparity_wrapper, "disparity_wrapper"},
    {516, fft_wrapper, "fft_wrapper"},
    {517, sha_wrapper, "sha_wrapper"},
    {518, fft_wrapper, "fft_wrapper"},
    {519, bandwidth_wrapper, "bandwidth_wrapper"},
    // Scenario 66
    {520, dijkstra_wrapper, "dijkstra_wrapper"},
    {521, bandwidth_wrapper, "bandwidth_wrapper"},
    {522, bandwidth_wrapper, "bandwidth_wrapper"},
    {523, fft_wrapper, "fft_wrapper"},
    {524, fft_wrapper, "fft_wrapper"},
    {525, fft_wrapper, "fft_wrapper"},
    {526, bandwidth_wrapper, "bandwidth_wrapper"},
    {527, sha_wrapper, "sha_wrapper"},
    // Scenario 67
    {528, dijkstra_wrapper, "dijkstra_wrapper"},
    {529, bandwidth_wrapper, "bandwidth_wrapper"},
    {530, bandwidth_wrapper, "bandwidth_wrapper"},
    {531, sorting_wrapper, "sorting_wrapper"},
    {532, sha_wrapper, "sha_wrapper"},
    {533, sorting_wrapper, "sorting_wrapper"},
    {534, dijkstra_wrapper, "dijkstra_wrapper"},
    {535, mser_wrapper, "mser_wrapper"},
    // Scenario 68
    {536, dijkstra_wrapper, "dijkstra_wrapper"},
    {537, bandwidth_wrapper, "bandwidth_wrapper"},
    {538, dijkstra_wrapper, "dijkstra_wrapper"},
    {539, bandwidth_wrapper, "bandwidth_wrapper"},
    {540, disparity_wrapper, "disparity_wrapper"},
    {541, disparity_wrapper, "disparity_wrapper"},
    {542, disparity_wrapper, "disparity_wrapper"},
    {543, mser_wrapper, "mser_wrapper"},
    // Scenario 69
    {544, dijkstra_wrapper, "dijkstra_wrapper"},
    {545, bandwidth_wrapper, "bandwidth_wrapper"},
    {546, disparity_wrapper, "disparity_wrapper"},
    {547, dijkstra_wrapper, "dijkstra_wrapper"},
    {548, fft_wrapper, "fft_wrapper"},
    {549, mser_wrapper, "mser_wrapper"},
    {550, sorting_wrapper, "sorting_wrapper"},
    {551, fft_wrapper, "fft_wrapper"},
    // Scenario 70
    {552, dijkstra_wrapper, "dijkstra_wrapper"},
    {553, bandwidth_wrapper, "bandwidth_wrapper"},
    {554, disparity_wrapper, "disparity_wrapper"},
    {555, fft_wrapper, "fft_wrapper"},
    {556, bandwidth_wrapper, "bandwidth_wrapper"},
    {557, disparity_wrapper, "disparity_wrapper"},
    {558, fft_wrapper, "fft_wrapper"},
    {559, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 71
    {560, dijkstra_wrapper, "dijkstra_wrapper"},
    {561, bandwidth_wrapper, "bandwidth_wrapper"},
    {562, disparity_wrapper, "disparity_wrapper"},
    {563, sha_wrapper, "sha_wrapper"},
    {564, fft_wrapper, "fft_wrapper"},
    {565, sorting_wrapper, "sorting_wrapper"},
    {566, sha_wrapper, "sha_wrapper"},
    {567, mser_wrapper, "mser_wrapper"},
    // Scenario 72
    {568, dijkstra_wrapper, "dijkstra_wrapper"},
    {569, bandwidth_wrapper, "bandwidth_wrapper"},
    {570, disparity_wrapper, "disparity_wrapper"},
    {571, sha_wrapper, "sha_wrapper"},
    {572, sorting_wrapper, "sorting_wrapper"},
    {573, sorting_wrapper, "sorting_wrapper"},
    {574, disparity_wrapper, "disparity_wrapper"},
    {575, bandwidth_wrapper, "bandwidth_wrapper"},
    // Scenario 73
    {576, dijkstra_wrapper, "dijkstra_wrapper"},
    {577, bandwidth_wrapper, "bandwidth_wrapper"},
    {578, fft_wrapper, "fft_wrapper"},
    {579, mser_wrapper, "mser_wrapper"},
    {580, bandwidth_wrapper, "bandwidth_wrapper"},
    {581, sorting_wrapper, "sorting_wrapper"},
    {582, disparity_wrapper, "disparity_wrapper"},
    {583, bandwidth_wrapper, "bandwidth_wrapper"},
    // Scenario 74
    {584, dijkstra_wrapper, "dijkstra_wrapper"},
    {585, bandwidth_wrapper, "bandwidth_wrapper"},
    {586, sha_wrapper, "sha_wrapper"},
    {587, dijkstra_wrapper, "dijkstra_wrapper"},
    {588, bandwidth_wrapper, "bandwidth_wrapper"},
    {589, fft_wrapper, "fft_wrapper"},
    {590, sha_wrapper, "sha_wrapper"},
    {591, fft_wrapper, "fft_wrapper"},
    // Scenario 75
    {592, dijkstra_wrapper, "dijkstra_wrapper"},
    {593, bandwidth_wrapper, "bandwidth_wrapper"},
    {594, sha_wrapper, "sha_wrapper"},
    {595, mser_wrapper, "mser_wrapper"},
    {596, mser_wrapper, "mser_wrapper"},
    {597, fft_wrapper, "fft_wrapper"},
    {598, sorting_wrapper, "sorting_wrapper"},
    {599, mser_wrapper, "mser_wrapper"},
    // Scenario 76
    {600, dijkstra_wrapper, "dijkstra_wrapper"},
    {601, bandwidth_wrapper, "bandwidth_wrapper"},
    {602, sorting_wrapper, "sorting_wrapper"},
    {603, fft_wrapper, "fft_wrapper"},
    {604, fft_wrapper, "fft_wrapper"},
    {605, sorting_wrapper, "sorting_wrapper"},
    {606, bandwidth_wrapper, "bandwidth_wrapper"},
    {607, fft_wrapper, "fft_wrapper"},
    // Scenario 77
    {608, dijkstra_wrapper, "dijkstra_wrapper"},
    {609, dijkstra_wrapper, "dijkstra_wrapper"},
    {610, bandwidth_wrapper, "bandwidth_wrapper"},
    {611, disparity_wrapper, "disparity_wrapper"},
    {612, bandwidth_wrapper, "bandwidth_wrapper"},
    {613, disparity_wrapper, "disparity_wrapper"},
    {614, fft_wrapper, "fft_wrapper"},
    {615, sha_wrapper, "sha_wrapper"},
    // Scenario 78
    {616, dijkstra_wrapper, "dijkstra_wrapper"},
    {617, dijkstra_wrapper, "dijkstra_wrapper"},
    {618, dijkstra_wrapper, "dijkstra_wrapper"},
    {619, bandwidth_wrapper, "bandwidth_wrapper"},
    {620, sorting_wrapper, "sorting_wrapper"},
    {621, fft_wrapper, "fft_wrapper"},
    {622, bandwidth_wrapper, "bandwidth_wrapper"},
    {623, sorting_wrapper, "sorting_wrapper"},
    // Scenario 79
    {624, dijkstra_wrapper, "dijkstra_wrapper"},
    {625, dijkstra_wrapper, "dijkstra_wrapper"},
    {626, dijkstra_wrapper, "dijkstra_wrapper"},
    {627, mser_wrapper, "mser_wrapper"},
    {628, disparity_wrapper, "disparity_wrapper"},
    {629, disparity_wrapper, "disparity_wrapper"},
    {630, bandwidth_wrapper, "bandwidth_wrapper"},
    {631, bandwidth_wrapper, "bandwidth_wrapper"},
    // Scenario 80
    {632, dijkstra_wrapper, "dijkstra_wrapper"},
    {633, dijkstra_wrapper, "dijkstra_wrapper"},
    {634, disparity_wrapper, "disparity_wrapper"},
    {635, disparity_wrapper, "disparity_wrapper"},
    {636, fft_wrapper, "fft_wrapper"},
    {637, sha_wrapper, "sha_wrapper"},
    {638, sha_wrapper, "sha_wrapper"},
    {639, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 81
    {640, dijkstra_wrapper, "dijkstra_wrapper"},
    {641, dijkstra_wrapper, "dijkstra_wrapper"},
    {642, disparity_wrapper, "disparity_wrapper"},
    {643, sha_wrapper, "sha_wrapper"},
    {644, disparity_wrapper, "disparity_wrapper"},
    {645, disparity_wrapper, "disparity_wrapper"},
    {646, mser_wrapper, "mser_wrapper"},
    {647, disparity_wrapper, "disparity_wrapper"},
    // Scenario 82
    {648, dijkstra_wrapper, "dijkstra_wrapper"},
    {649, dijkstra_wrapper, "dijkstra_wrapper"},
    {650, fft_wrapper, "fft_wrapper"},
    {651, fft_wrapper, "fft_wrapper"},
    {652, bandwidth_wrapper, "bandwidth_wrapper"},
    {653, dijkstra_wrapper, "dijkstra_wrapper"},
    {654, sorting_wrapper, "sorting_wrapper"},
    {655, sorting_wrapper, "sorting_wrapper"},
    // Scenario 83
    {656, dijkstra_wrapper, "dijkstra_wrapper"},
    {657, dijkstra_wrapper, "dijkstra_wrapper"},
    {658, sha_wrapper, "sha_wrapper"},
    {659, dijkstra_wrapper, "dijkstra_wrapper"},
    {660, fft_wrapper, "fft_wrapper"},
    {661, dijkstra_wrapper, "dijkstra_wrapper"},
    {662, sorting_wrapper, "sorting_wrapper"},
    {663, sorting_wrapper, "sorting_wrapper"},
    // Scenario 84
    {664, dijkstra_wrapper, "dijkstra_wrapper"},
    {665, dijkstra_wrapper, "dijkstra_wrapper"},
    {666, sha_wrapper, "sha_wrapper"},
    {667, disparity_wrapper, "disparity_wrapper"},
    {668, sorting_wrapper, "sorting_wrapper"},
    {669, fft_wrapper, "fft_wrapper"},
    {670, bandwidth_wrapper, "bandwidth_wrapper"},
    {671, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 85
    {672, dijkstra_wrapper, "dijkstra_wrapper"},
    {673, dijkstra_wrapper, "dijkstra_wrapper"},
    {674, sorting_wrapper, "sorting_wrapper"},
    {675, dijkstra_wrapper, "dijkstra_wrapper"},
    {676, mser_wrapper, "mser_wrapper"},
    {677, bandwidth_wrapper, "bandwidth_wrapper"},
    {678, mser_wrapper, "mser_wrapper"},
    {679, sha_wrapper, "sha_wrapper"},
    // Scenario 86
    {680, dijkstra_wrapper, "dijkstra_wrapper"},
    {681, dijkstra_wrapper, "dijkstra_wrapper"},
    {682, sorting_wrapper, "sorting_wrapper"},
    {683, sha_wrapper, "sha_wrapper"},
    {684, fft_wrapper, "fft_wrapper"},
    {685, sorting_wrapper, "sorting_wrapper"},
    {686, sorting_wrapper, "sorting_wrapper"},
    {687, sha_wrapper, "sha_wrapper"},
    // Scenario 87
    {688, dijkstra_wrapper, "dijkstra_wrapper"},
    {689, disparity_wrapper, "disparity_wrapper"},
    {690, bandwidth_wrapper, "bandwidth_wrapper"},
    {691, bandwidth_wrapper, "bandwidth_wrapper"},
    {692, mser_wrapper, "mser_wrapper"},
    {693, bandwidth_wrapper, "bandwidth_wrapper"},
    {694, bandwidth_wrapper, "bandwidth_wrapper"},
    {695, mser_wrapper, "mser_wrapper"},
    // Scenario 88
    {696, dijkstra_wrapper, "dijkstra_wrapper"},
    {697, disparity_wrapper, "disparity_wrapper"},
    {698, dijkstra_wrapper, "dijkstra_wrapper"},
    {699, fft_wrapper, "fft_wrapper"},
    {700, disparity_wrapper, "disparity_wrapper"},
    {701, bandwidth_wrapper, "bandwidth_wrapper"},
    {702, dijkstra_wrapper, "dijkstra_wrapper"},
    {703, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 89
    {704, dijkstra_wrapper, "dijkstra_wrapper"},
    {705, disparity_wrapper, "disparity_wrapper"},
    {706, dijkstra_wrapper, "dijkstra_wrapper"},
    {707, mser_wrapper, "mser_wrapper"},
    {708, bandwidth_wrapper, "bandwidth_wrapper"},
    {709, sha_wrapper, "sha_wrapper"},
    {710, disparity_wrapper, "disparity_wrapper"},
    {711, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 90
    {712, dijkstra_wrapper, "dijkstra_wrapper"},
    {713, disparity_wrapper, "disparity_wrapper"},
    {714, dijkstra_wrapper, "dijkstra_wrapper"},
    {715, sha_wrapper, "sha_wrapper"},
    {716, bandwidth_wrapper, "bandwidth_wrapper"},
    {717, bandwidth_wrapper, "bandwidth_wrapper"},
    {718, mser_wrapper, "mser_wrapper"},
    {719, sha_wrapper, "sha_wrapper"},
    // Scenario 91
    {720, dijkstra_wrapper, "dijkstra_wrapper"},
    {721, disparity_wrapper, "disparity_wrapper"},
    {722, disparity_wrapper, "disparity_wrapper"},
    {723, disparity_wrapper, "disparity_wrapper"},
    {724, dijkstra_wrapper, "dijkstra_wrapper"},
    {725, sha_wrapper, "sha_wrapper"},
    {726, fft_wrapper, "fft_wrapper"},
    {727, bandwidth_wrapper, "bandwidth_wrapper"},
    // Scenario 92
    {728, dijkstra_wrapper, "dijkstra_wrapper"},
    {729, disparity_wrapper, "disparity_wrapper"},
    {730, disparity_wrapper, "disparity_wrapper"},
    {731, sha_wrapper, "sha_wrapper"},
    {732, disparity_wrapper, "disparity_wrapper"},
    {733, mser_wrapper, "mser_wrapper"},
    {734, mser_wrapper, "mser_wrapper"},
    {735, sha_wrapper, "sha_wrapper"},
    // Scenario 93
    {736, dijkstra_wrapper, "dijkstra_wrapper"},
    {737, disparity_wrapper, "disparity_wrapper"},
    {738, fft_wrapper, "fft_wrapper"},
    {739, mser_wrapper, "mser_wrapper"},
    {740, disparity_wrapper, "disparity_wrapper"},
    {741, bandwidth_wrapper, "bandwidth_wrapper"},
    {742, dijkstra_wrapper, "dijkstra_wrapper"},
    {743, fft_wrapper, "fft_wrapper"},
    // Scenario 94
    {744, dijkstra_wrapper, "dijkstra_wrapper"},
    {745, disparity_wrapper, "disparity_wrapper"},
    {746, mser_wrapper, "mser_wrapper"},
    {747, fft_wrapper, "fft_wrapper"},
    {748, sha_wrapper, "sha_wrapper"},
    {749, dijkstra_wrapper, "dijkstra_wrapper"},
    {750, sha_wrapper, "sha_wrapper"},
    {751, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 95
    {752, dijkstra_wrapper, "dijkstra_wrapper"},
    {753, disparity_wrapper, "disparity_wrapper"},
    {754, mser_wrapper, "mser_wrapper"},
    {755, sorting_wrapper, "sorting_wrapper"},
    {756, sha_wrapper, "sha_wrapper"},
    {757, dijkstra_wrapper, "dijkstra_wrapper"},
    {758, mser_wrapper, "mser_wrapper"},
    {759, fft_wrapper, "fft_wrapper"},
    // Scenario 96
    {760, dijkstra_wrapper, "dijkstra_wrapper"},
    {761, disparity_wrapper, "disparity_wrapper"},
    {762, mser_wrapper, "mser_wrapper"},
    {763, sorting_wrapper, "sorting_wrapper"},
    {764, sha_wrapper, "sha_wrapper"},
    {765, sorting_wrapper, "sorting_wrapper"},
    {766, sorting_wrapper, "sorting_wrapper"},
    {767, fft_wrapper, "fft_wrapper"},
    // Scenario 97
    {768, dijkstra_wrapper, "dijkstra_wrapper"},
    {769, disparity_wrapper, "disparity_wrapper"},
    {770, sha_wrapper, "sha_wrapper"},
    {771, fft_wrapper, "fft_wrapper"},
    {772, mser_wrapper, "mser_wrapper"},
    {773, dijkstra_wrapper, "dijkstra_wrapper"},
    {774, dijkstra_wrapper, "dijkstra_wrapper"},
    {775, fft_wrapper, "fft_wrapper"},
    // Scenario 98
    {776, dijkstra_wrapper, "dijkstra_wrapper"},
    {777, disparity_wrapper, "disparity_wrapper"},
    {778, sha_wrapper, "sha_wrapper"},
    {779, mser_wrapper, "mser_wrapper"},
    {780, sha_wrapper, "sha_wrapper"},
    {781, mser_wrapper, "mser_wrapper"},
    {782, sha_wrapper, "sha_wrapper"},
    {783, bandwidth_wrapper, "bandwidth_wrapper"},
    // Scenario 99
    {784, dijkstra_wrapper, "dijkstra_wrapper"},
    {785, disparity_wrapper, "disparity_wrapper"},
    {786, sorting_wrapper, "sorting_wrapper"},
    {787, disparity_wrapper, "disparity_wrapper"},
    {788, disparity_wrapper, "disparity_wrapper"},
    {789, bandwidth_wrapper, "bandwidth_wrapper"},
    {790, disparity_wrapper, "disparity_wrapper"},
    {791, sorting_wrapper, "sorting_wrapper"},
    // Scenario 100
    {792, dijkstra_wrapper, "dijkstra_wrapper"},
    {793, fft_wrapper, "fft_wrapper"},
    {794, bandwidth_wrapper, "bandwidth_wrapper"},
    {795, bandwidth_wrapper, "bandwidth_wrapper"},
    {796, dijkstra_wrapper, "dijkstra_wrapper"},
    {797, disparity_wrapper, "disparity_wrapper"},
    {798, dijkstra_wrapper, "dijkstra_wrapper"},
    {799, disparity_wrapper, "disparity_wrapper"},
    // Scenario 101
    {800, dijkstra_wrapper, "dijkstra_wrapper"},
    {801, fft_wrapper, "fft_wrapper"},
    {802, disparity_wrapper, "disparity_wrapper"},
    {803, mser_wrapper, "mser_wrapper"},
    {804, disparity_wrapper, "disparity_wrapper"},
    {805, mser_wrapper, "mser_wrapper"},
    {806, dijkstra_wrapper, "dijkstra_wrapper"},
    {807, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 102
    {808, dijkstra_wrapper, "dijkstra_wrapper"},
    {809, fft_wrapper, "fft_wrapper"},
    {810, fft_wrapper, "fft_wrapper"},
    {811, bandwidth_wrapper, "bandwidth_wrapper"},
    {812, mser_wrapper, "mser_wrapper"},
    {813, fft_wrapper, "fft_wrapper"},
    {814, fft_wrapper, "fft_wrapper"},
    {815, sorting_wrapper, "sorting_wrapper"},
    // Scenario 103
    {816, dijkstra_wrapper, "dijkstra_wrapper"},
    {817, fft_wrapper, "fft_wrapper"},
    {818, fft_wrapper, "fft_wrapper"},
    {819, fft_wrapper, "fft_wrapper"},
    {820, bandwidth_wrapper, "bandwidth_wrapper"},
    {821, bandwidth_wrapper, "bandwidth_wrapper"},
    {822, disparity_wrapper, "disparity_wrapper"},
    {823, mser_wrapper, "mser_wrapper"},
    // Scenario 104
    {824, dijkstra_wrapper, "dijkstra_wrapper"},
    {825, fft_wrapper, "fft_wrapper"},
    {826, fft_wrapper, "fft_wrapper"},
    {827, fft_wrapper, "fft_wrapper"},
    {828, sha_wrapper, "sha_wrapper"},
    {829, sorting_wrapper, "sorting_wrapper"},
    {830, fft_wrapper, "fft_wrapper"},
    {831, fft_wrapper, "fft_wrapper"},
    // Scenario 105
    {832, dijkstra_wrapper, "dijkstra_wrapper"},
    {833, fft_wrapper, "fft_wrapper"},
    {834, fft_wrapper, "fft_wrapper"},
    {835, mser_wrapper, "mser_wrapper"},
    {836, fft_wrapper, "fft_wrapper"},
    {837, mser_wrapper, "mser_wrapper"},
    {838, sha_wrapper, "sha_wrapper"},
    {839, disparity_wrapper, "disparity_wrapper"},
    // Scenario 106
    {840, dijkstra_wrapper, "dijkstra_wrapper"},
    {841, fft_wrapper, "fft_wrapper"},
    {842, mser_wrapper, "mser_wrapper"},
    {843, bandwidth_wrapper, "bandwidth_wrapper"},
    {844, sha_wrapper, "sha_wrapper"},
    {845, sha_wrapper, "sha_wrapper"},
    {846, sha_wrapper, "sha_wrapper"},
    {847, sorting_wrapper, "sorting_wrapper"},
    // Scenario 107
    {848, dijkstra_wrapper, "dijkstra_wrapper"},
    {849, fft_wrapper, "fft_wrapper"},
    {850, mser_wrapper, "mser_wrapper"},
    {851, bandwidth_wrapper, "bandwidth_wrapper"},
    {852, sorting_wrapper, "sorting_wrapper"},
    {853, dijkstra_wrapper, "dijkstra_wrapper"},
    {854, disparity_wrapper, "disparity_wrapper"},
    {855, sha_wrapper, "sha_wrapper"},
    // Scenario 108
    {856, dijkstra_wrapper, "dijkstra_wrapper"},
    {857, fft_wrapper, "fft_wrapper"},
    {858, mser_wrapper, "mser_wrapper"},
    {859, disparity_wrapper, "disparity_wrapper"},
    {860, dijkstra_wrapper, "dijkstra_wrapper"},
    {861, bandwidth_wrapper, "bandwidth_wrapper"},
    {862, dijkstra_wrapper, "dijkstra_wrapper"},
    {863, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 109
    {864, dijkstra_wrapper, "dijkstra_wrapper"},
    {865, fft_wrapper, "fft_wrapper"},
    {866, sha_wrapper, "sha_wrapper"},
    {867, sha_wrapper, "sha_wrapper"},
    {868, disparity_wrapper, "disparity_wrapper"},
    {869, sorting_wrapper, "sorting_wrapper"},
    {870, dijkstra_wrapper, "dijkstra_wrapper"},
    {871, disparity_wrapper, "disparity_wrapper"},
    // Scenario 110
    {872, dijkstra_wrapper, "dijkstra_wrapper"},
    {873, fft_wrapper, "fft_wrapper"},
    {874, sorting_wrapper, "sorting_wrapper"},
    {875, bandwidth_wrapper, "bandwidth_wrapper"},
    {876, mser_wrapper, "mser_wrapper"},
    {877, mser_wrapper, "mser_wrapper"},
    {878, mser_wrapper, "mser_wrapper"},
    {879, mser_wrapper, "mser_wrapper"},
    // Scenario 111
    {880, dijkstra_wrapper, "dijkstra_wrapper"},
    {881, mser_wrapper, "mser_wrapper"},
    {882, disparity_wrapper, "disparity_wrapper"},
    {883, bandwidth_wrapper, "bandwidth_wrapper"},
    {884, bandwidth_wrapper, "bandwidth_wrapper"},
    {885, bandwidth_wrapper, "bandwidth_wrapper"},
    {886, sha_wrapper, "sha_wrapper"},
    {887, mser_wrapper, "mser_wrapper"},
    // Scenario 112
    {888, dijkstra_wrapper, "dijkstra_wrapper"},
    {889, mser_wrapper, "mser_wrapper"},
    {890, disparity_wrapper, "disparity_wrapper"},
    {891, bandwidth_wrapper, "bandwidth_wrapper"},
    {892, dijkstra_wrapper, "dijkstra_wrapper"},
    {893, sorting_wrapper, "sorting_wrapper"},
    {894, fft_wrapper, "fft_wrapper"},
    {895, sha_wrapper, "sha_wrapper"},
    // Scenario 113
    {896, dijkstra_wrapper, "dijkstra_wrapper"},
    {897, mser_wrapper, "mser_wrapper"},
    {898, disparity_wrapper, "disparity_wrapper"},
    {899, fft_wrapper, "fft_wrapper"},
    {900, sorting_wrapper, "sorting_wrapper"},
    {901, bandwidth_wrapper, "bandwidth_wrapper"},
    {902, sorting_wrapper, "sorting_wrapper"},
    {903, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 114
    {904, dijkstra_wrapper, "dijkstra_wrapper"},
    {905, mser_wrapper, "mser_wrapper"},
    {906, disparity_wrapper, "disparity_wrapper"},
    {907, mser_wrapper, "mser_wrapper"},
    {908, fft_wrapper, "fft_wrapper"},
    {909, bandwidth_wrapper, "bandwidth_wrapper"},
    {910, sorting_wrapper, "sorting_wrapper"},
    {911, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 115
    {912, dijkstra_wrapper, "dijkstra_wrapper"},
    {913, mser_wrapper, "mser_wrapper"},
    {914, fft_wrapper, "fft_wrapper"},
    {915, disparity_wrapper, "disparity_wrapper"},
    {916, dijkstra_wrapper, "dijkstra_wrapper"},
    {917, mser_wrapper, "mser_wrapper"},
    {918, disparity_wrapper, "disparity_wrapper"},
    {919, mser_wrapper, "mser_wrapper"},
    // Scenario 116
    {920, dijkstra_wrapper, "dijkstra_wrapper"},
    {921, mser_wrapper, "mser_wrapper"},
    {922, mser_wrapper, "mser_wrapper"},
    {923, disparity_wrapper, "disparity_wrapper"},
    {924, sorting_wrapper, "sorting_wrapper"},
    {925, disparity_wrapper, "disparity_wrapper"},
    {926, bandwidth_wrapper, "bandwidth_wrapper"},
    {927, disparity_wrapper, "disparity_wrapper"},
    // Scenario 117
    {928, dijkstra_wrapper, "dijkstra_wrapper"},
    {929, mser_wrapper, "mser_wrapper"},
    {930, sha_wrapper, "sha_wrapper"},
    {931, disparity_wrapper, "disparity_wrapper"},
    {932, sha_wrapper, "sha_wrapper"},
    {933, sha_wrapper, "sha_wrapper"},
    {934, sha_wrapper, "sha_wrapper"},
    {935, bandwidth_wrapper, "bandwidth_wrapper"},
    // Scenario 118
    {936, dijkstra_wrapper, "dijkstra_wrapper"},
    {937, mser_wrapper, "mser_wrapper"},
    {938, sha_wrapper, "sha_wrapper"},
    {939, mser_wrapper, "mser_wrapper"},
    {940, sha_wrapper, "sha_wrapper"},
    {941, fft_wrapper, "fft_wrapper"},
    {942, sha_wrapper, "sha_wrapper"},
    {943, sha_wrapper, "sha_wrapper"},
    // Scenario 119
    {944, dijkstra_wrapper, "dijkstra_wrapper"},
    {945, mser_wrapper, "mser_wrapper"},
    {946, sorting_wrapper, "sorting_wrapper"},
    {947, fft_wrapper, "fft_wrapper"},
    {948, sha_wrapper, "sha_wrapper"},
    {949, mser_wrapper, "mser_wrapper"},
    {950, bandwidth_wrapper, "bandwidth_wrapper"},
    {951, sha_wrapper, "sha_wrapper"},
    // Scenario 120
    {952, dijkstra_wrapper, "dijkstra_wrapper"},
    {953, sha_wrapper, "sha_wrapper"},
    {954, bandwidth_wrapper, "bandwidth_wrapper"},
    {955, bandwidth_wrapper, "bandwidth_wrapper"},
    {956, mser_wrapper, "mser_wrapper"},
    {957, disparity_wrapper, "disparity_wrapper"},
    {958, mser_wrapper, "mser_wrapper"},
    {959, sha_wrapper, "sha_wrapper"},
    // Scenario 121
    {960, dijkstra_wrapper, "dijkstra_wrapper"},
    {961, sha_wrapper, "sha_wrapper"},
    {962, bandwidth_wrapper, "bandwidth_wrapper"},
    {963, sha_wrapper, "sha_wrapper"},
    {964, fft_wrapper, "fft_wrapper"},
    {965, bandwidth_wrapper, "bandwidth_wrapper"},
    {966, sorting_wrapper, "sorting_wrapper"},
    {967, disparity_wrapper, "disparity_wrapper"},
    // Scenario 122
    {968, dijkstra_wrapper, "dijkstra_wrapper"},
    {969, sha_wrapper, "sha_wrapper"},
    {970, bandwidth_wrapper, "bandwidth_wrapper"},
    {971, sorting_wrapper, "sorting_wrapper"},
    {972, mser_wrapper, "mser_wrapper"},
    {973, disparity_wrapper, "disparity_wrapper"},
    {974, bandwidth_wrapper, "bandwidth_wrapper"},
    {975, disparity_wrapper, "disparity_wrapper"},
    // Scenario 123
    {976, dijkstra_wrapper, "dijkstra_wrapper"},
    {977, sha_wrapper, "sha_wrapper"},
    {978, dijkstra_wrapper, "dijkstra_wrapper"},
    {979, sorting_wrapper, "sorting_wrapper"},
    {980, mser_wrapper, "mser_wrapper"},
    {981, fft_wrapper, "fft_wrapper"},
    {982, bandwidth_wrapper, "bandwidth_wrapper"},
    {983, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 124
    {984, dijkstra_wrapper, "dijkstra_wrapper"},
    {985, sha_wrapper, "sha_wrapper"},
    {986, disparity_wrapper, "disparity_wrapper"},
    {987, disparity_wrapper, "disparity_wrapper"},
    {988, sorting_wrapper, "sorting_wrapper"},
    {989, bandwidth_wrapper, "bandwidth_wrapper"},
    {990, disparity_wrapper, "disparity_wrapper"},
    {991, disparity_wrapper, "disparity_wrapper"},
    // Scenario 125
    {992, dijkstra_wrapper, "dijkstra_wrapper"},
    {993, sha_wrapper, "sha_wrapper"},
    {994, disparity_wrapper, "disparity_wrapper"},
    {995, sha_wrapper, "sha_wrapper"},
    {996, fft_wrapper, "fft_wrapper"},
    {997, sorting_wrapper, "sorting_wrapper"},
    {998, bandwidth_wrapper, "bandwidth_wrapper"},
    {999, bandwidth_wrapper, "bandwidth_wrapper"},
    // Scenario 126
    {1000, dijkstra_wrapper, "dijkstra_wrapper"},
    {1001, sha_wrapper, "sha_wrapper"},
    {1002, fft_wrapper, "fft_wrapper"},
    {1003, bandwidth_wrapper, "bandwidth_wrapper"},
    {1004, disparity_wrapper, "disparity_wrapper"},
    {1005, sha_wrapper, "sha_wrapper"},
    {1006, disparity_wrapper, "disparity_wrapper"},
    {1007, bandwidth_wrapper, "bandwidth_wrapper"},
    // Scenario 127
    {1008, dijkstra_wrapper, "dijkstra_wrapper"},
    {1009, sha_wrapper, "sha_wrapper"},
    {1010, fft_wrapper, "fft_wrapper"},
    {1011, bandwidth_wrapper, "bandwidth_wrapper"},
    {1012, sha_wrapper, "sha_wrapper"},
    {1013, dijkstra_wrapper, "dijkstra_wrapper"},
    {1014, fft_wrapper, "fft_wrapper"},
    {1015, bandwidth_wrapper, "bandwidth_wrapper"},
    // Scenario 128
    {1016, dijkstra_wrapper, "dijkstra_wrapper"},
    {1017, sha_wrapper, "sha_wrapper"},
    {1018, fft_wrapper, "fft_wrapper"},
    {1019, disparity_wrapper, "disparity_wrapper"},
    {1020, disparity_wrapper, "disparity_wrapper"},
    {1021, dijkstra_wrapper, "dijkstra_wrapper"},
    {1022, fft_wrapper, "fft_wrapper"},
    {1023, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 129
    {1024, dijkstra_wrapper, "dijkstra_wrapper"},
    {1025, sha_wrapper, "sha_wrapper"},
    {1026, fft_wrapper, "fft_wrapper"},
    {1027, fft_wrapper, "fft_wrapper"},
    {1028, bandwidth_wrapper, "bandwidth_wrapper"},
    {1029, mser_wrapper, "mser_wrapper"},
    {1030, dijkstra_wrapper, "dijkstra_wrapper"},
    {1031, sha_wrapper, "sha_wrapper"},
    // Scenario 130
    {1032, dijkstra_wrapper, "dijkstra_wrapper"},
    {1033, sha_wrapper, "sha_wrapper"},
    {1034, fft_wrapper, "fft_wrapper"},
    {1035, mser_wrapper, "mser_wrapper"},
    {1036, bandwidth_wrapper, "bandwidth_wrapper"},
    {1037, sha_wrapper, "sha_wrapper"},
    {1038, sha_wrapper, "sha_wrapper"},
    {1039, mser_wrapper, "mser_wrapper"},
    // Scenario 131
    {1040, dijkstra_wrapper, "dijkstra_wrapper"},
    {1041, sha_wrapper, "sha_wrapper"},
    {1042, mser_wrapper, "mser_wrapper"},
    {1043, bandwidth_wrapper, "bandwidth_wrapper"},
    {1044, dijkstra_wrapper, "dijkstra_wrapper"},
    {1045, mser_wrapper, "mser_wrapper"},
    {1046, dijkstra_wrapper, "dijkstra_wrapper"},
    {1047, disparity_wrapper, "disparity_wrapper"},
    // Scenario 132
    {1048, dijkstra_wrapper, "dijkstra_wrapper"},
    {1049, sha_wrapper, "sha_wrapper"},
    {1050, sha_wrapper, "sha_wrapper"},
    {1051, dijkstra_wrapper, "dijkstra_wrapper"},
    {1052, mser_wrapper, "mser_wrapper"},
    {1053, fft_wrapper, "fft_wrapper"},
    {1054, bandwidth_wrapper, "bandwidth_wrapper"},
    {1055, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 133
    {1056, dijkstra_wrapper, "dijkstra_wrapper"},
    {1057, sha_wrapper, "sha_wrapper"},
    {1058, sha_wrapper, "sha_wrapper"},
    {1059, fft_wrapper, "fft_wrapper"},
    {1060, fft_wrapper, "fft_wrapper"},
    {1061, sorting_wrapper, "sorting_wrapper"},
    {1062, fft_wrapper, "fft_wrapper"},
    {1063, bandwidth_wrapper, "bandwidth_wrapper"},
    // Scenario 134
    {1064, dijkstra_wrapper, "dijkstra_wrapper"},
    {1065, sha_wrapper, "sha_wrapper"},
    {1066, sorting_wrapper, "sorting_wrapper"},
    {1067, disparity_wrapper, "disparity_wrapper"},
    {1068, bandwidth_wrapper, "bandwidth_wrapper"},
    {1069, dijkstra_wrapper, "dijkstra_wrapper"},
    {1070, disparity_wrapper, "disparity_wrapper"},
    {1071, sha_wrapper, "sha_wrapper"},
    // Scenario 135
    {1072, dijkstra_wrapper, "dijkstra_wrapper"},
    {1073, sha_wrapper, "sha_wrapper"},
    {1074, sorting_wrapper, "sorting_wrapper"},
    {1075, fft_wrapper, "fft_wrapper"},
    {1076, sha_wrapper, "sha_wrapper"},
    {1077, fft_wrapper, "fft_wrapper"},
    {1078, sha_wrapper, "sha_wrapper"},
    {1079, sha_wrapper, "sha_wrapper"},
    // Scenario 136
    {1080, dijkstra_wrapper, "dijkstra_wrapper"},
    {1081, sorting_wrapper, "sorting_wrapper"},
    {1082, bandwidth_wrapper, "bandwidth_wrapper"},
    {1083, fft_wrapper, "fft_wrapper"},
    {1084, dijkstra_wrapper, "dijkstra_wrapper"},
    {1085, dijkstra_wrapper, "dijkstra_wrapper"},
    {1086, bandwidth_wrapper, "bandwidth_wrapper"},
    {1087, bandwidth_wrapper, "bandwidth_wrapper"},
    // Scenario 137
    {1088, dijkstra_wrapper, "dijkstra_wrapper"},
    {1089, sorting_wrapper, "sorting_wrapper"},
    {1090, bandwidth_wrapper, "bandwidth_wrapper"},
    {1091, mser_wrapper, "mser_wrapper"},
    {1092, sorting_wrapper, "sorting_wrapper"},
    {1093, disparity_wrapper, "disparity_wrapper"},
    {1094, dijkstra_wrapper, "dijkstra_wrapper"},
    {1095, mser_wrapper, "mser_wrapper"},
    // Scenario 138
    {1096, dijkstra_wrapper, "dijkstra_wrapper"},
    {1097, sorting_wrapper, "sorting_wrapper"},
    {1098, bandwidth_wrapper, "bandwidth_wrapper"},
    {1099, sha_wrapper, "sha_wrapper"},
    {1100, mser_wrapper, "mser_wrapper"},
    {1101, mser_wrapper, "mser_wrapper"},
    {1102, mser_wrapper, "mser_wrapper"},
    {1103, mser_wrapper, "mser_wrapper"},
    // Scenario 139
    {1104, dijkstra_wrapper, "dijkstra_wrapper"},
    {1105, sorting_wrapper, "sorting_wrapper"},
    {1106, bandwidth_wrapper, "bandwidth_wrapper"},
    {1107, sorting_wrapper, "sorting_wrapper"},
    {1108, fft_wrapper, "fft_wrapper"},
    {1109, bandwidth_wrapper, "bandwidth_wrapper"},
    {1110, disparity_wrapper, "disparity_wrapper"},
    {1111, fft_wrapper, "fft_wrapper"},
    // Scenario 140
    {1112, dijkstra_wrapper, "dijkstra_wrapper"},
    {1113, sorting_wrapper, "sorting_wrapper"},
    {1114, dijkstra_wrapper, "dijkstra_wrapper"},
    {1115, fft_wrapper, "fft_wrapper"},
    {1116, bandwidth_wrapper, "bandwidth_wrapper"},
    {1117, fft_wrapper, "fft_wrapper"},
    {1118, bandwidth_wrapper, "bandwidth_wrapper"},
    {1119, disparity_wrapper, "disparity_wrapper"},
    // Scenario 141
    {1120, dijkstra_wrapper, "dijkstra_wrapper"},
    {1121, sorting_wrapper, "sorting_wrapper"},
    {1122, dijkstra_wrapper, "dijkstra_wrapper"},
    {1123, fft_wrapper, "fft_wrapper"},
    {1124, sha_wrapper, "sha_wrapper"},
    {1125, bandwidth_wrapper, "bandwidth_wrapper"},
    {1126, mser_wrapper, "mser_wrapper"},
    {1127, mser_wrapper, "mser_wrapper"},
    // Scenario 142
    {1128, dijkstra_wrapper, "dijkstra_wrapper"},
    {1129, sorting_wrapper, "sorting_wrapper"},
    {1130, disparity_wrapper, "disparity_wrapper"},
    {1131, bandwidth_wrapper, "bandwidth_wrapper"},
    {1132, fft_wrapper, "fft_wrapper"},
    {1133, dijkstra_wrapper, "dijkstra_wrapper"},
    {1134, mser_wrapper, "mser_wrapper"},
    {1135, fft_wrapper, "fft_wrapper"},
    // Scenario 143
    {1136, dijkstra_wrapper, "dijkstra_wrapper"},
    {1137, sorting_wrapper, "sorting_wrapper"},
    {1138, fft_wrapper, "fft_wrapper"},
    {1139, sorting_wrapper, "sorting_wrapper"},
    {1140, fft_wrapper, "fft_wrapper"},
    {1141, bandwidth_wrapper, "bandwidth_wrapper"},
    {1142, disparity_wrapper, "disparity_wrapper"},
    {1143, bandwidth_wrapper, "bandwidth_wrapper"},
    // Scenario 144
    {1144, dijkstra_wrapper, "dijkstra_wrapper"},
    {1145, sorting_wrapper, "sorting_wrapper"},
    {1146, mser_wrapper, "mser_wrapper"},
    {1147, dijkstra_wrapper, "dijkstra_wrapper"},
    {1148, fft_wrapper, "fft_wrapper"},
    {1149, fft_wrapper, "fft_wrapper"},
    {1150, fft_wrapper, "fft_wrapper"},
    {1151, mser_wrapper, "mser_wrapper"},
    // Scenario 145
    {1152, dijkstra_wrapper, "dijkstra_wrapper"},
    {1153, sorting_wrapper, "sorting_wrapper"},
    {1154, sha_wrapper, "sha_wrapper"},
    {1155, disparity_wrapper, "disparity_wrapper"},
    {1156, dijkstra_wrapper, "dijkstra_wrapper"},
    {1157, mser_wrapper, "mser_wrapper"},
    {1158, disparity_wrapper, "disparity_wrapper"},
    {1159, disparity_wrapper, "disparity_wrapper"},
    // Scenario 146
    {1160, dijkstra_wrapper, "dijkstra_wrapper"},
    {1161, sorting_wrapper, "sorting_wrapper"},
    {1162, sha_wrapper, "sha_wrapper"},
    {1163, disparity_wrapper, "disparity_wrapper"},
    {1164, mser_wrapper, "mser_wrapper"},
    {1165, mser_wrapper, "mser_wrapper"},
    {1166, bandwidth_wrapper, "bandwidth_wrapper"},
    {1167, disparity_wrapper, "disparity_wrapper"},
    // Scenario 147
    {1168, dijkstra_wrapper, "dijkstra_wrapper"},
    {1169, sorting_wrapper, "sorting_wrapper"},
    {1170, sorting_wrapper, "sorting_wrapper"},
    {1171, fft_wrapper, "fft_wrapper"},
    {1172, sorting_wrapper, "sorting_wrapper"},
    {1173, fft_wrapper, "fft_wrapper"},
    {1174, disparity_wrapper, "disparity_wrapper"},
    {1175, mser_wrapper, "mser_wrapper"},
    // Scenario 148
    {1176, disparity_wrapper, "disparity_wrapper"},
    {1177, bandwidth_wrapper, "bandwidth_wrapper"},
    {1178, bandwidth_wrapper, "bandwidth_wrapper"},
    {1179, disparity_wrapper, "disparity_wrapper"},
    {1180, sha_wrapper, "sha_wrapper"},
    {1181, mser_wrapper, "mser_wrapper"},
    {1182, disparity_wrapper, "disparity_wrapper"},
    {1183, sha_wrapper, "sha_wrapper"},
    // Scenario 149
    {1184, disparity_wrapper, "disparity_wrapper"},
    {1185, bandwidth_wrapper, "bandwidth_wrapper"},
    {1186, bandwidth_wrapper, "bandwidth_wrapper"},
    {1187, mser_wrapper, "mser_wrapper"},
    {1188, dijkstra_wrapper, "dijkstra_wrapper"},
    {1189, dijkstra_wrapper, "dijkstra_wrapper"},
    {1190, fft_wrapper, "fft_wrapper"},
    {1191, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 150
    {1192, disparity_wrapper, "disparity_wrapper"},
    {1193, bandwidth_wrapper, "bandwidth_wrapper"},
    {1194, dijkstra_wrapper, "dijkstra_wrapper"},
    {1195, dijkstra_wrapper, "dijkstra_wrapper"},
    {1196, sorting_wrapper, "sorting_wrapper"},
    {1197, fft_wrapper, "fft_wrapper"},
    {1198, disparity_wrapper, "disparity_wrapper"},
    {1199, mser_wrapper, "mser_wrapper"},
    // Scenario 151
    {1200, disparity_wrapper, "disparity_wrapper"},
    {1201, bandwidth_wrapper, "bandwidth_wrapper"},
    {1202, dijkstra_wrapper, "dijkstra_wrapper"},
    {1203, disparity_wrapper, "disparity_wrapper"},
    {1204, bandwidth_wrapper, "bandwidth_wrapper"},
    {1205, sorting_wrapper, "sorting_wrapper"},
    {1206, sorting_wrapper, "sorting_wrapper"},
    {1207, sha_wrapper, "sha_wrapper"},
    // Scenario 152
    {1208, disparity_wrapper, "disparity_wrapper"},
    {1209, bandwidth_wrapper, "bandwidth_wrapper"},
    {1210, dijkstra_wrapper, "dijkstra_wrapper"},
    {1211, fft_wrapper, "fft_wrapper"},
    {1212, dijkstra_wrapper, "dijkstra_wrapper"},
    {1213, dijkstra_wrapper, "dijkstra_wrapper"},
    {1214, sorting_wrapper, "sorting_wrapper"},
    {1215, dijkstra_wrapper, "dijkstra_wrapper"},
    // Scenario 153
    {1216, disparity_wrapper, "disparity_wrapper"},
    {1217, bandwidth_wrapper, "bandwidth_wrapper"},
    {1218, dijkstra_wrapper, "dijkstra_wrapper"},
    {1219, mser_wrapper, "mser_wrapper"},
    {1220, dijkstra_wrapper, "dijkstra_wrapper"},
    {1221, dijkstra_wrapper, "dijkstra_wrapper"},
    {1222, bandwidth_wrapper, "bandwidth_wrapper"},
    {1223, sorting_wrapper, "sorting_wrapper"},
    // Scenario 154
    {1224, disparity_wrapper, "disparity_wrapper"},
    {1225, bandwidth_wrapper, "bandwidth_wrapper"},
    {1226, dijkstra_wrapper, "dijkstra_wrapper"},
    {1227, mser_wrapper, "mser_wrapper"},
    {1228, dijkstra_wrapper, "dijkstra_wrapper"},
    {1229, disparity_wrapper, "disparity_wrapper"},
    {1230, sorting_wrapper, "sorting_wrapper"},
    {1231, fft_wrapper, "fft_wrapper"},
    // Scenario 155
    {1232, disparity_wrapper, "disparity_wrapper"},
    {1233, bandwidth_wrapper, "bandwidth_wrapper"},
    {1234, disparity_wrapper, "disparity_wrapper"},
    {1235, sha_wrapper, "sha_wrapper"},
    {1236, mser_wrapper, "mser_wrapper"},
    {1237, fft_wrapper, "fft_wrapper"},
    {1238, dijkstra_wrapper, "dijkstra_wrapper"},
    {1239, disparity_wrapper, "disparity_wrapper"},
    // Scenario 156
    {1240, disparity_wrapper, "disparity_wrapper"},
    {1241, bandwidth_wrapper, "bandwidth_wrapper"},
    {1242, fft_wrapper, "fft_wrapper"},
    {1243, disparity_wrapper, "disparity_wrapper"},
    {1244, fft_wrapper, "fft_wrapper"},
    {1245, dijkstra_wrapper, "dijkstra_wrapper"},
    {1246, sha_wrapper, "sha_wrapper"},
    {1247, sha_wrapper, "sha_wrapper"},
    // Scenario 157
    {1248, disparity_wrapper, "disparity_wrapper"},
    {1249, bandwidth_wrapper, "bandwidth_wrapper"},
    {1250, sha_wrapper, "sha_wrapper"},
    {1251, disparity_wrapper, "disparity_wrapper"},
    {1252, dijkstra_wrapper, "dijkstra_wrapper"},
    {1253, sha_wrapper, "sha_wrapper"},
    {1254, dijkstra_wrapper, "dijkstra_wrapper"},
    {1255, sorting_wrapper, "sorting_wrapper"},
    // Scenario 158
    {1256, disparity_wrapper, "disparity_wrapper"},
    {1257, bandwidth_wrapper, "bandwidth_wrapper"},
    {1258, sha_wrapper, "sha_wrapper"},
    {1259, mser_wrapper, "mser_wrapper"},
    {1260, disparity_wrapper, "disparity_wrapper"},
    {1261, fft_wrapper, "fft_wrapper"},
    {1262, bandwidth_wrapper, "bandwidth_wrapper"},
    {1263, bandwidth_wrapper, "bandwidth_wrapper"},
    // Scenario 159
    {1264, disparity_wrapper, "disparity_wrapper"},
    {1265, bandwidth_wrapper, "bandwidth_wrapper"},
    {1266, sorting_wrapper, "sorting_wrapper"},
    {1267, sha_wrapper, "sha_wrapper"},
    {1268, fft_wrapper, "fft_wrapper"},
    {1269, sha_wrapper, "sha_wrapper"},
    {1270, sorting_wrapper, "sorting_wrapper"},
    {1271, bandwidth_wrapper, "bandwidth_wrapper"},
    // Scenario 160
    {1272, disparity_wrapper, "disparity_wrapper"},
    {1273, bandwidth_wrapper, "bandwidth_wrapper"},
    {1274, sorting_wrapper, "sorting_wrapper"},
    {1275, sorting_wrapper, "sorting_wrapper"},
    {1276, bandwidth_wrapper, "bandwidth_wrapper"},
    {1277, dijkstra_wrapper, "dijkstra_wrapper"},
    {1278, sorting_wrapper, "sorting_wrapper"},
    {1279, sha_wrapper, "sha_wrapper"},
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
#include "../inc/bench_functions.h"
#include <stdint.h>

#include "../inc/data.h"

#define array_size 128
volatile int a = 12345;
volatile int b = 6789;
volatile int c = 42;
volatile int d = 17;
volatile int array[array_size] = {
    53,  8,   27,  74,  19,  91,  42,  66,  3,   58,  95,  12,  81,  33,  60,
    4,   77,  23,  88,  49,  1,   63,  38,  99,  15,  70,  29,  84,  45,  9,
    51,  22,  78,  36,  93,  17,  68,  25,  80,  40,  7,   56,  31,  89,  14,
    73,  28,  85,  47,  6,   54,  21,  79,  34,  96,  11,  64,  26,  82,  43,
    5,   59,  32,  90,  16,  75,  24,  87,  48,  2,   52,  20,  76,  37,  94,
    18,  69,  30,  86,  46,  10,  62,  35,  97,  13,  71,  41,  67,  39,  92,
    57,  44,  100, 61,  83,  50,  72,  65,  55,  98,  110, 121, 105, 118, 102,
    127, 108, 115, 103, 123, 113, 107, 119, 101, 126, 104, 112, 122, 106, 117,
    111, 125, 109, 116, 114, 124, 120, 128};

// helper for some functions
void copy_array(const volatile int origin[], int *destiny, int size) {
  for (int i = 0; i < size; ++i) {
    destiny[i] = origin[i];
  }
}
////////////

void empty_wrapper() {
  while (1) {
    printf("\n\n\n\nthis should never happen\n\n\n\n");
  }
}

// void bubble_sort(int numbers[], int array_size);
// void selection_sort(int numbers[], int array_size);
// void shell_sort(int numbers[], int array_size);
// void insertion_sort(int numbers[], int array_size);
// void q_sort(int numbers[], int left, int right);
// void quick_sort(int numbers[], int array_size);

/* /\*----------------------------  function
 * ------------------------------------ */
/*  Name: bubble_sort */
/*  Purpose: Sort given array using bubble sort algorithm */
/*  Pre: Array a must be of atleast array_size */
/*  Post: Sorts the array a  */
/*  Arguments: a is the array that needs to be sorted and array_size is the size
 */
/*             of the array */
/*  Returns: void */
/*  Calls: None */
/*  Source: http://www.cs.princeton.edu/~ah/alg_anim/gawain-4.0/BubbleSort.html
 */
/*  ----------------------------------------------------------------------------*\/
 */
volatile int my_bubble_sort() {
  int i, j, tmp;
  int numbers[array_size];
  copy_array(array, numbers, array_size);

  for (i = 0; i < array_size; i++) {

    for (j = 0; j < (array_size - 1 - i); j++) {

      if (numbers[j + 1] < numbers[j]) {

        tmp = numbers[j];
        numbers[j] = numbers[j + 1];
        numbers[j + 1] = tmp;

      } // end if

    } // end for j

  } // end for i

  return 0;
}

/* /\*----------------------------  function
 * ------------------------------------ */
/*  Name: selection_sort */
/*  Purpose: Sort given array using selection sort algorithm  */
/*  Pre: Array must be of atleast array_size */
/*  Post: Sorts the array upto array_size */
/*  Arguments: numbers is the array that needs to be sorted and array_size is
 * the */
/*             index upto which the elements will be sorted */
/*  Returns: void */
/*  Calls: None */
/*  Source: http://linux.wku.edu/~lamonml/algor/sort/selection.html */
/*  ----------------------------------------------------------------------------*\/
 */
volatile int my_selection_sort() {
  int i, j;
  int min, temp;
  int numbers[array_size];
  copy_array(array, numbers, array_size);

  for (i = 0; i < array_size - 1; i++) {
    min = i;
    for (j = i + 1; j < array_size; j++) {
      if (numbers[j] < numbers[min])
        min = j;
    }
    temp = numbers[i];
    numbers[i] = numbers[min];
    numbers[min] = temp;
  }
  return 0;
}

/* /\*----------------------------  function
 * ------------------------------------ */
/*  Name: shell_sort */
/*  Purpose: Sort given array using shell sort algorithm */
/*  Pre: numbers must be of atleast array_size */
/*  Post: Sorts the array  */
/*  Arguments: number is the array that needs to be sorted and array_size is the
 */
/*             size of the array */
/*  Returns: void */
/*  Calls: None */
/*  Source: http://linux.wku.edu/~lamonml/algor/sort/shell.html */
/*  ----------------------------------------------------------------------------*\/
 */
volatile int my_shell_sort() {
  int i, j, increment, temp;
  int numbers[array_size];
  copy_array(array, numbers, array_size);

  increment = 3;
  while (increment > 0) {
    for (i = 0; i < array_size; i++) {
      j = i;
      temp = numbers[i];
      while ((j >= increment) && (numbers[j - increment] > temp)) {
        numbers[j] = numbers[j - increment];
        j = j - increment;
      }
      numbers[j] = temp;
    }
    if (increment / 2 != 0)
      increment = increment / 2;
    else if (increment == 1)
      increment = 0;
    else
      increment = 1;
  }
  return 0;
}

/* /\*----------------------------  function
 * ------------------------------------ */
/*  Name: insertion_sort */
/*  Purpose: Sort given array using Insertion sort algorithm */
/*  Pre: numbers must be atleast of array_size */
/*  Post: Sorts the array */
/*  Arguments: numbers is the array that needs to be sorted and array_size is
 * the */
/*             size of the array */
/*  Returns: void */
/*  Calls: None */
/*  Source: http://linux.wku.edu/~lamonml/algor/sort/insertion.html */
/*  ----------------------------------------------------------------------------*\/
 */
volatile int my_insertion_sort() {
  int i, j, index;
  int numbers[array_size];
  copy_array(array, numbers, array_size);

  for (i = 1; i < array_size; i++) {
    index = numbers[i];
    j = i;
    while ((j > 0) && (numbers[j - 1] > index)) {
      numbers[j] = numbers[j - 1];
      j = j - 1;
    }
    numbers[j] = index;
  }
  return 0;
}

/* /\*----------------------------  function
 * ------------------------------------ */
/*  Name: q_sort */
/*  Purpose: Sort array using quick sort algorithm */
/*  Pre: left must be less than right */
/*  Post: Breaks the array into half and calls sort function again recursively
 */
/*  Arguments: number is the array and left and right are the index up to which
 */
/*             array needs to be sorted */
/*  Returns: void */
/*  Calls: q_sort */
/*  Source: http://linux.wku.edu/~lamonml/algor/sort/quick.html */
/*  ----------------------------------------------------------------------------*\/
 */
volatile int my_q_sort(int v[], int left, int right) {
  int pivot, l_hold, r_hold;

  l_hold = left;
  r_hold = right;
  pivot = v[left];
  while (left < right) {
    while ((v[right] >= pivot) && (left < right))
      right--;
    if (left != right) {
      v[left] = v[right];
      left++;
    }
    while ((v[left] <= pivot) && (left < right))
      left++;
    if (left != right) {
      v[right] = v[left];
      right--;
    }
  }
  v[left] = pivot;
  pivot = left;
  left = l_hold;
  right = r_hold;
  if (left < pivot)
    my_q_sort(v, left, pivot - 1);
  if (right > pivot)
    my_q_sort(v, pivot + 1, right);

  return 0;
}

/* /\*----------------------------  function
 * ------------------------------------ */
/*  Name: quick_sort */
/*  Purpose: To call the recursive q_sort method */
/*  Pre: numbers must be of atleast array_size */
/*  Post: Calls the q_sort method to sort the array */
/*  Arguments: number is the array that needs to be sorted and array_size is the
 */
/*             array size */
/*  Returns: void */
/*  Calls: q_sort */
/*  ----------------------------------------------------------------------------*\/
 */
volatile int my_quick_sort() {
  int numbers[array_size];
  copy_array(array, numbers, array_size);
  my_q_sort(numbers, 0, array_size - 1);
  return 0;
}

///////////////////
int *g_mem_ptr = NULL; // bandwidth
I2D *It = NULL;

void init_data() {
  // bandwitdh
  g_mem_ptr = (int *)malloc(BW_DEFAULT_ALLOC_SIZE);
  memset((char *)g_mem_ptr, 1, BW_DEFAULT_ALLOC_SIZE);
  for (uint64_t i = 0; i < BW_DEFAULT_ALLOC_SIZE / sizeof(int); i++) {
    g_mem_ptr[i] = i;
  }

  // mser
  int i, j, k;
  I2D *I;
  // I2D *It;
  int rows = 196, cols = 98;

  I = (I2D *)mserb;
  rows = I->height;
  cols = I->width;

  It = (I2D *)mserb1;

  k = 0;
  for (i = 0; i < cols; i++) {
    for (j = 0; j < rows; j++) {
      asubsref(It, k++) = subsref(I, j, i);
    }
  }
}

void free_data() {
  // bandwitdh
  free(g_mem_ptr);
}

void bandwidth_wrapper() {
  static volatile uint64_t sum = 0;
  sum += bench_read(g_mem_ptr);
  sum += bench_write(g_mem_ptr);

  return;
}

void dijkstra_wrapper() {
  int i, j, k;

  for (i = 0, j = NUM_NODES / 2; i < 100; i++, j++) {
    j = j % NUM_NODES;
    dijkstra(i, j);
  }

  return;
}

// -
void disparity_wrapper() {
  I2D *imleft = (I2D *)img1;
  I2D *imright = (I2D *)img2;
  int WIN_SZ = 8, SHIFT = 64;
  char signature[2] = {66, 77};
  short int bits_per_pixel = 24;

  // Check if images are valid BMP images.
  if (imleft->height <= 0 || imleft->width <= 0 || signature[0] != 'B' ||
      signature[1] != 'M' || (bits_per_pixel != 24 && bits_per_pixel != 8)) {
    return;
  }

  if (imright->height <= 0 || imright->width <= 0 || signature[0] != 'B' ||
      signature[1] != 'M' || (bits_per_pixel != 24 && bits_per_pixel != 8)) {
    return;
  }

  I2D *retDisparity = getDisparity(imleft, imright, WIN_SZ, SHIFT);
  return;
}

// -
void fft_wrapper() {
  static uint32_t A_re[NUM_POINTS];
  static uint32_t A_im[NUM_POINTS];
  static uint32_t W_re[NUM_POINTS / 2];
  static uint32_t W_im[NUM_POINTS / 2];

  int n = NUM_POINTS;
  init_array(n, A_re, A_im);
  compute_W(n, W_re, W_im);
  fft(n, A_re, A_im, W_re, W_im);
  permute_bitrev(n, A_re, A_im);

  return;
}

// -
void mser_wrapper() {
  //
  mser(It, 2);
  return;
}

// -
void qsort_wrapper() {
  struct my3DVertexStruct array[QSORT_MAXARRAY];

  int numRows = sizeof(qsort_input_data) / sizeof(qsort_input_data[0]);
  int count = 0;

  // Process the array instead of reading from a file
  for (int i = 0; i < numRows && count < QSORT_MAXARRAY; i++) {
    array[count].x = qsort_input_data[i][0];
    array[count].y = qsort_input_data[i][1];
    array[count].z = qsort_input_data[i][2];
    array[count].distance = (array[count].x * array[count].x) +
                            (array[count].y * array[count].y) +
                            (array[count].z * array[count].z);
    count++;
  }

  qsort(array, count, sizeof(struct my3DVertexStruct), qsort_compare);
  return;
}

void sha_wrapper() {
  SHA_INFO sha_info;
  sha_stream(&sha_info);
  return;
}

void sorting_wrapper() {
  int orig[MAX_SORTING], copy[MAX_SORTING], i;

  void *function[NUM_SORT] = {&selection_sort, &quick_sort,     &shell_sort,
                              &stdlib_qsort,   &insertion_sort, &bubble_sort};
  char *sort_name[NUM_SORT] = {"Selection sort", "Quicksort",
                               "Shellsort",      "Qsort",
                               "Insertion sort", "Bubble sort"};

  fill_array(orig, MAX_SORTING);

  for (i = 0; i < NUM_SORT; i++) {
    execute_sort(orig, copy, MAX_SORTING, sort_name[i], function[i]);
  }

  return;
}

// formula_t used_budget_formula = EWMA_FORMULA;

const char *benchmark_get_formula_name(Benchmark *b) {
  switch (b->formula) {
  case EWMA_FORMULA:
    return "EWMA";
    break;
  case SW_FORMULA:
    return "SW";
    break;
  case AFC_FORMULA:
    return "AFC";
    break;
  case AMBP_FORMULA:
    return "AMBP";
    break;
  case PIC_FORMULA:
    return "PIC";
    break;
  // case LR_FORMULA:
  //   return "LR";
  //   break;
  case EWMA_V2_FORMULA:
    return "EWMA_v2";
    break;
  case SW_V2_FORMULA:
    return "SW_v2";
    break;
  case AFC_V2_FORMULA:
    return "AFC_v2";
    break;
  case AMBP_V2_FORMULA:
    return "AMBP_v2";
    break;
  case LR_V2_FORMULA:
    return "LR_v2";
    break;
  case PIC_V2_FORMULA:
    return "PIC_v2";
    break;
  default:
    printf("invalid formula name %d, returning UNDEFINED\n", formula);
    return "UNDEFINED";
    break;
  }
}
