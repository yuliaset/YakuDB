#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hdf5.h>

#define TOTAL_TILES 14
#define MAX_TOTAL 15
#define MAX_FLAG 2

#define SUIT_TYPES 9
#define HONOR_TYPES 7

#define MAX_SUIT_CONFIGS 15000
#define MAX_HONOR_CONFIGS 1500

typedef struct {
    int counts[SUIT_TYPES];
} SuitConfig;

typedef struct {
    int counts[HONOR_TYPES];
} HonorConfig;

SuitConfig suitConfigs[MAX_TOTAL][MAX_FLAG][MAX_SUIT_CONFIGS];
int suitConfigCount[MAX_TOTAL][MAX_FLAG] = {0};

HonorConfig honorConfigs[MAX_TOTAL][MAX_FLAG][MAX_HONOR_CONFIGS];
int honorConfigCount[MAX_TOTAL][MAX_FLAG] = {0};

int checkMeldSuit(int counts[SUIT_TYPES]) {
    int i;
    for (i = 0; i < SUIT_TYPES; i++) {
        if (counts[i] > 0)
            break;
    }
    if (i == SUIT_TYPES)
        return 1;
    if (counts[i] >= 3) {
        counts[i] -= 3;
        if (checkMeldSuit(counts)) {
            counts[i] += 3;
            return 1;
        }
        counts[i] += 3;
    }
    if (i <= 6 && counts[i+1] > 0 && counts[i+2] > 0) {
        counts[i]--; counts[i+1]--; counts[i+2]--;
        if (checkMeldSuit(counts)) {
            counts[i]++; counts[i+1]++; counts[i+2]++;
            return 1;
        }
        counts[i]++; counts[i+1]++; counts[i+2]++;
    }
    return 0;
}

int checkPairSuit(int counts[SUIT_TYPES]) {
    for (int i = 0; i < SUIT_TYPES; i++) {
        if (counts[i] >= 2) {
            counts[i] -= 2;
            if (checkMeldSuit(counts)) {
                counts[i] += 2;
                return 1;
            }
            counts[i] += 2;
        }
    }
    return 0;
}

int checkMeldHonor(int counts[HONOR_TYPES]) {
    int i;
    for (i = 0; i < HONOR_TYPES; i++) {
        if (counts[i] > 0)
            break;
    }
    if (i == HONOR_TYPES)
        return 1;
    if (counts[i] >= 3) {
        counts[i] -= 3;
        if (checkMeldHonor(counts)) {
            counts[i] += 3;
            return 1;
        }
        counts[i] += 3;
    }
    return 0;
}

int checkPairHonor(int counts[HONOR_TYPES]) {
    for (int i = 0; i < HONOR_TYPES; i++) {
        if (counts[i] >= 2) {
            counts[i] -= 2;
            if (checkMeldHonor(counts)) {
                counts[i] += 2;
                return 1;
            }
            counts[i] += 2;
        }
    }
    return 0;
}

void enumerateSuitRec(int idx, int currentSum, int target, int counts[SUIT_TYPES]) {
    if (idx == SUIT_TYPES) {
        if (currentSum <= target) {
            if (currentSum % 3 == 0) {
                int temp[SUIT_TYPES];
                memcpy(temp, counts, sizeof(int) * SUIT_TYPES);
                if (checkMeldSuit(temp)) {
                    int t = currentSum;
                    int flag = 0;
                    if (suitConfigCount[t][flag] < MAX_SUIT_CONFIGS) {
                        memcpy(suitConfigs[t][flag][suitConfigCount[t][flag]].counts, 
                               counts, sizeof(int) * SUIT_TYPES);
                        suitConfigCount[t][flag]++;
                    }
                }
            }
            if (currentSum % 3 == 2) {
                int temp[SUIT_TYPES];
                memcpy(temp, counts, sizeof(int) * SUIT_TYPES);
                if (checkPairSuit(temp)) {
                    int t = currentSum;
                    int flag = 1;
                    if (suitConfigCount[t][flag] < MAX_SUIT_CONFIGS) {
                        memcpy(suitConfigs[t][flag][suitConfigCount[t][flag]].counts, 
                               counts, sizeof(int) * SUIT_TYPES);
                        suitConfigCount[t][flag]++;
                    }
                }
            }
        }
        return;
    }
    for (int c = 0; c <= 4; c++) {
        if (currentSum + c > target)
            break;
        counts[idx] = c;
        enumerateSuitRec(idx + 1, currentSum + c, target, counts);
    }
}

void enumerateSuit() {
    int counts[SUIT_TYPES] = {0};
    enumerateSuitRec(0, 0, TOTAL_TILES, counts);
}

void enumerateHonorRec(int idx, int currentSum, int target, int counts[HONOR_TYPES]) {
    if (idx == HONOR_TYPES) {
        if (currentSum <= target) {
            if (currentSum % 3 == 0) {
                int temp[HONOR_TYPES];
                memcpy(temp, counts, sizeof(int) * HONOR_TYPES);
                if (checkMeldHonor(temp)) {
                    int t = currentSum;
                    int flag = 0;
                    if (honorConfigCount[t][flag] < MAX_HONOR_CONFIGS) {
                        memcpy(honorConfigs[t][flag][honorConfigCount[t][flag]].counts,
                               counts, sizeof(int) * HONOR_TYPES);
                        honorConfigCount[t][flag]++;
                    }
                }
            }
            if (currentSum % 3 == 2) {
                int temp[HONOR_TYPES];
                memcpy(temp, counts, sizeof(int) * HONOR_TYPES);
                if (checkPairHonor(temp)) {
                    int t = currentSum;
                    int flag = 1;
                    if (honorConfigCount[t][flag] < MAX_HONOR_CONFIGS) {
                        memcpy(honorConfigs[t][flag][honorConfigCount[t][flag]].counts,
                               counts, sizeof(int) * HONOR_TYPES);
                        honorConfigCount[t][flag]++;
                    }
                }
            }
        }
        return;
    }
    for (int c = 0; c <= 4; c++) {
        if (currentSum + c > target)
            break;
        counts[idx] = c;
        enumerateHonorRec(idx + 1, currentSum + c, target, counts);
    }
}

void enumerateHonor() {
    int counts[HONOR_TYPES] = {0};
    enumerateHonorRec(0, 0, TOTAL_TILES, counts);
}

unsigned long long computeCompleteHands() {
    unsigned long long totalCompleteHands = 0;
    for (int t0 = 0; t0 <= TOTAL_TILES; t0++) {
        for (int t1 = 0; t1 <= TOTAL_TILES; t1++) {
            for (int t2 = 0; t2 <= TOTAL_TILES; t2++) {
                int t3 = TOTAL_TILES - t0 - t1 - t2;
                if (t3 < 0 || t3 > TOTAL_TILES)
                    continue;
                for (int pairGroup = 0; pairGroup < 4; pairGroup++) {
                    int valid = 1;
                    int meldCount = 0;
                    int ts[4] = {t0, t1, t2, t3};
                    int reqFlag[4];
                    for (int g = 0; g < 4; g++) {
                        if (g == pairGroup) {
                            if (ts[g] % 3 != 2) { valid = 0; break; }
                            reqFlag[g] = 1;
                            meldCount += (ts[g] - 2) / 3;
                        } else {
                            if (ts[g] % 3 != 0) { valid = 0; break; }
                            reqFlag[g] = 0;
                            meldCount += ts[g] / 3;
                        }
                    }
                    if (!valid || meldCount != 4)
                        continue;
                    unsigned long long ways = 1;
                    ways *= suitConfigCount[t0][reqFlag[0]];
                    ways *= suitConfigCount[t1][reqFlag[1]];
                    ways *= suitConfigCount[t2][reqFlag[2]];
                    ways *= honorConfigCount[t3][reqFlag[3]];
                    totalCompleteHands += ways;
                }
            }
        }
    }
    return totalCompleteHands;
}

unsigned long long nCr(int n, int r) {
    unsigned long long result = 1;
    for (int i = 1; i <= r; i++) {
        result = result * (n - r + i) / i;
    }
    return result;
}

unsigned long long computeChiitoitsu() {

    return nCr(34, 7);
}

unsigned long long computeKokushiMusou() {

    return 13;
}

unsigned long long computeTotalHands() {

    const int n_tile_types = 34;
    const int hand_size = 14;
    static unsigned long long dp[35][15] = {0};
    dp[0][0] = 1ULL;
    for (int i = 1; i <= n_tile_types; i++) {
        for (int j = 0; j <= hand_size; j++) {
            for (int used = 0; used <= 4; used++) {
                if (used <= j) {
                    dp[i][j] += dp[i - 1][j - used];
                }
            }
        }
    }
    return dp[n_tile_types][hand_size];
}

static int *standardHands = NULL;  
static int standardIndex = 0;      
static unsigned long long standardCount = 0;

static int *chiitoiHands = NULL;
static int chiitoiIndex = 0;
static unsigned long long chiitoiCount = 0;

static int *kokushiHands = NULL;
static int kokushiIndex = 0;
static unsigned long long kokushiCount = 0;

void fillStandardWinningHands(void) {
    standardIndex = 0; 
    for (int t0 = 0; t0 <= TOTAL_TILES; t0++) {
        for (int t1 = 0; t1 <= TOTAL_TILES; t1++) {
            for (int t2 = 0; t2 <= TOTAL_TILES; t2++) {
                int t3 = TOTAL_TILES - t0 - t1 - t2;
                if (t3 < 0 || t3 > TOTAL_TILES)
                    continue;
                for (int pairGroup = 0; pairGroup < 4; pairGroup++) {
                    int valid = 1;
                    int meldCount = 0;
                    int ts[4] = {t0, t1, t2, t3};
                    int reqFlag[4];
                    for (int g = 0; g < 4; g++) {
                        if (g == pairGroup) {
                            if (ts[g] % 3 != 2) { valid = 0; break; }
                            reqFlag[g] = 1;
                            meldCount += (ts[g] - 2) / 3;
                        } else {
                            if (ts[g] % 3 != 0) { valid = 0; break; }
                            reqFlag[g] = 0;
                            meldCount += ts[g] / 3;
                        }
                    }
                    if (!valid || meldCount != 4)
                        continue;

                    for (int i0 = 0; i0 < suitConfigCount[ts[0]][reqFlag[0]]; i0++) {
                        for (int i1 = 0; i1 < suitConfigCount[ts[1]][reqFlag[1]]; i1++) {
                            for (int i2 = 0; i2 < suitConfigCount[ts[2]][reqFlag[2]]; i2++) {
                                for (int i3 = 0; i3 < honorConfigCount[ts[3]][reqFlag[3]]; i3++) {
                                    int hand[34] = {0};

                                    for (int k = 0; k < SUIT_TYPES; k++) {
                                        hand[k] = suitConfigs[ts[0]][reqFlag[0]][i0].counts[k];
                                    }

                                    for (int k = 0; k < SUIT_TYPES; k++) {
                                        hand[SUIT_TYPES + k] = suitConfigs[ts[1]][reqFlag[1]][i1].counts[k];
                                    }

                                    for (int k = 0; k < SUIT_TYPES; k++) {
                                        hand[2 * SUIT_TYPES + k] = suitConfigs[ts[2]][reqFlag[2]][i2].counts[k];
                                    }

                                    for (int k = 0; k < HONOR_TYPES; k++) {
                                        hand[3 * SUIT_TYPES + k] = honorConfigs[ts[3]][reqFlag[3]][i3].counts[k];
                                    }

                                    memcpy(&standardHands[standardIndex * 34], hand, 34 * sizeof(int));
                                    standardIndex++;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

static int subset[7] = {0};
void fillChiitoiHandsRec(int start, int depth) {
    if (depth == 7) {
        int hand[34] = {0};
        for (int i = 0; i < 7; i++) {
            hand[subset[i]] = 2;
        }
        memcpy(&chiitoiHands[chiitoiIndex * 34], hand, 34 * sizeof(int));
        chiitoiIndex++;
        return;
    }
    for (int i = start; i < 34; i++) {
        subset[depth] = i;
        fillChiitoiHandsRec(i + 1, depth + 1);
    }
}
void fillChiitoiHands(void) {
    chiitoiIndex = 0;
    fillChiitoiHandsRec(0, 0);
}

void fillKokushiMusouHands(void) {
    int terminals[13] = {0, 8, 9, 17, 18, 26, 27, 28, 29, 30, 31, 32, 33};
    kokushiIndex = 0;
    for (int extra = 0; extra < 13; extra++) {
        int hand[34] = {0};
        for (int i = 0; i < 13; i++) {
            hand[terminals[i]] = 1;
        }
        hand[terminals[extra]] = 2; 
        memcpy(&kokushiHands[kokushiIndex * 34], hand, 34 * sizeof(int));
        kokushiIndex++;
    }
}

void writeDatasetInt2D(hid_t file_id, const char *datasetName,
                       const int *data, hsize_t nRows, hsize_t nCols) {

    hsize_t dims[2] = {nRows, nCols};
    hid_t dataspace_id = H5Screate_simple(2, dims, NULL);

    hid_t dataset_id = H5Dcreate2(file_id, datasetName, H5T_NATIVE_INT, 
                                  dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    H5Dwrite(dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);

    H5Dclose(dataset_id);
    H5Sclose(dataspace_id);
}

int main(void) {

    unsigned long long totalHands = computeTotalHands();
    enumerateSuit();
    enumerateHonor();
    unsigned long long standardComplete = computeCompleteHands();
    unsigned long long chiitoi = computeChiitoitsu();
    unsigned long long kokushi = computeKokushiMusou();
    unsigned long long overallWinning = standardComplete + chiitoi + kokushi;

    standardCount = standardComplete;  
    chiitoiCount = chiitoi;           
    kokushiCount = kokushi;           

    standardHands = (int*) malloc(sizeof(int) * (size_t)standardCount * 34);
    chiitoiHands  = (int*) malloc(sizeof(int) * (size_t)chiitoiCount  * 34);
    kokushiHands  = (int*) malloc(sizeof(int) * (size_t)kokushiCount  * 34);

    if (!standardHands || !chiitoiHands || !kokushiHands) {
        fprintf(stderr, "Memmory allocation error.\n");
        return 1;
    }

    fillStandardWinningHands(); 
    fillChiitoiHands();         
    fillKokushiMusouHands();    

    hid_t file_id = H5Fcreate("dataset.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    if (file_id < 0) {
        fprintf(stderr, "Error creating HDF5 file.\n");
        return 1;
    }

    if (standardCount > 0) {
        writeDatasetInt2D(file_id, "standardWinningHands",
                          standardHands, (hsize_t)standardCount, 34);
    }

    if (chiitoiCount > 0) {
        writeDatasetInt2D(file_id, "chiitoiHands",
                          chiitoiHands, (hsize_t)chiitoiCount, 34);
    }

    if (kokushiCount > 0) {
        writeDatasetInt2D(file_id, "kokushiHands",
                          kokushiHands, (hsize_t)kokushiCount, 34);
    }

    {

        hid_t attr_space = H5Screate(H5S_SCALAR);
        hid_t attr_id = H5Acreate2(file_id, "overallWinning", H5T_NATIVE_ULLONG,
                                   attr_space, H5P_DEFAULT, H5P_DEFAULT);
        H5Awrite(attr_id, H5T_NATIVE_ULLONG, &overallWinning);
        H5Aclose(attr_id);
        H5Sclose(attr_space);

    }

    H5Fclose(file_id);

    free(standardHands);
    free(chiitoiHands);
    free(kokushiHands);

    return 0;
}