#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <iomanip>
#define _WIN32_WINNT 0x0A00 

// Platform-specific sleep implementation to avoid thread issues
#ifdef _WIN32
    #include <windows.h>
    #define SLEEP(ms) Sleep(ms)
#else
    #include <unistd.h>
    #define SLEEP(ms) usleep((ms) * 1000)
#endif

class SortingVisualizer {
private:
    std::vector<int> arr;
    int size;
    int maxVal;
    int delay; // milliseconds

public:
    SortingVisualizer(int n = 20, int delayMs = 100) : size(n), delay(delayMs) {
        generateRandomArray();
    }

    void generateRandomArray() {
        arr.clear();
        arr.resize(size);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, size);

        for (int i = 0; i < size; i++) {
            arr[i] = dis(gen);
        }

        maxVal = *std::max_element(arr.begin(), arr.end());
    }

    void display(const std::string& algorithm = "", int step = -1,
        int highlight1 = -1, int highlight2 = -1) {
        // Clear screen (works on most terminals)
        std::cout << "\033[2J\033[H";

        if (!algorithm.empty()) {
            std::cout << "Algorithm: " << algorithm;
            if (step >= 0) std::cout << " | Step: " << step;
            std::cout << "\n\n";
        }

        // Display bars vertically
        for (int row = maxVal; row >= 1; row--) {
            for (int i = 0; i < size; i++) {
                if (arr[i] >= row) {
                    if (i == highlight1) {
                        std::cout << "\033[31m██\033[0m"; // Red for first highlight
                    }
                    else if (i == highlight2) {
                        std::cout << "\033[32m██\033[0m"; // Green for second highlight
                    }
                    else {
                        std::cout << "██";
                    }
                }
                else {
                    std::cout << "  ";
                }
            }
            std::cout << "\n";
        }

        // Display array values
        std::cout << "\n";
        for (int i = 0; i < size; i++) {
            std::cout << std::setw(2) << arr[i];
        }
        std::cout << "\n\n";

        SLEEP(delay);
    }

    void bubbleSort() {
        std::cout << "Starting Bubble Sort...\n";
        SLEEP(1000);

        int step = 0;
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                display("Bubble Sort", step++, j, j + 1);

                if (arr[j] > arr[j + 1]) {
                    std::swap(arr[j], arr[j + 1]);
                    display("Bubble Sort", step++, j, j + 1);
                }
            }
        }
        display("Bubble Sort - COMPLETED!");
    }

    void selectionSort() {
        std::cout << "Starting Selection Sort...\n";
        SLEEP(1000);

        int step = 0;
        for (int i = 0; i < size - 1; i++) {
            int minIdx = i;
            display("Selection Sort", step++, i, minIdx);

            for (int j = i + 1; j < size; j++) {
                display("Selection Sort", step++, j, minIdx);
                if (arr[j] < arr[minIdx]) {
                    minIdx = j;
                }
            }

            if (minIdx != i) {
                std::swap(arr[i], arr[minIdx]);
                display("Selection Sort", step++, i, minIdx);
            }
        }
        display("Selection Sort - COMPLETED!");
    }

    void insertionSort() {
        std::cout << "Starting Insertion Sort...\n";
        SLEEP(1000);

        int step = 0;
        for (int i = 1; i < size; i++) {
            int key = arr[i];
            int j = i - 1;

            display("Insertion Sort", step++, i, -1);

            while (j >= 0 && arr[j] > key) {
                arr[j + 1] = arr[j];
                display("Insertion Sort", step++, j, j + 1);
                j--;
            }
            arr[j + 1] = key;
            display("Insertion Sort", step++, j + 1, -1);
        }
        display("Insertion Sort - COMPLETED!");
    }

    void quickSort() {
        std::cout << "Starting Quick Sort...\n";
        SLEEP(1000);

        int step = 0;
        quickSortHelper(0, size - 1, step);
        display("Quick Sort - COMPLETED!");
    }

private:
    void quickSortHelper(int low, int high, int& step) {
        if (low < high) {
            int pi = partition(low, high, step);
            quickSortHelper(low, pi - 1, step);
            quickSortHelper(pi + 1, high, step);
        }
    }

    int partition(int low, int high, int& step) {
        int pivot = arr[high];
        int i = low - 1;

        for (int j = low; j < high; j++) {
            display("Quick Sort", step++, j, high);
            if (arr[j] < pivot) {
                i++;
                std::swap(arr[i], arr[j]);
                display("Quick Sort", step++, i, j);
            }
        }
        std::swap(arr[i + 1], arr[high]);
        display("Quick Sort", step++, i + 1, high);
        return i + 1;
    }

public:
    void inputCustomArray() {
        std::cout << "Enter the number of elements (5-50): ";
        int n;
        std::cin >> n;

        if (n < 5 || n > 50) {
            std::cout << "Invalid size! Please enter between 5 and 50.\n";
            return;
        }

        size = n;
        arr.clear();
        arr.resize(size);

        std::cout << "Enter " << size << " integers separated by spaces:\n";
        for (int i = 0; i < size; i++) {
            std::cin >> arr[i];
            if (arr[i] <= 0) {
                std::cout << "Warning: Non-positive values will be displayed as empty bars!\n";
            }
        }

        maxVal = *std::max_element(arr.begin(), arr.end());
        if (maxVal <= 0) {
            maxVal = 1; // Prevent issues with non-positive arrays
        }

        std::cout << "Custom array created successfully!\n";
        display("Custom Array");
    }

    void menu() {
        int choice;

        do {
            std::cout << "\n\033[36m=== SORTING VISUALIZER ===\033[0m\n";
            std::cout << "1. Generate New Random Array\n";
            std::cout << "2. Input Custom Array\n";
            std::cout << "3. Bubble Sort\n";
            std::cout << "4. Selection Sort\n";
            std::cout << "5. Insertion Sort\n";
            std::cout << "6. Quick Sort\n";
            std::cout << "7. Change Array Size (for random generation)\n";
            std::cout << "8. Change Animation Speed\n";
            std::cout << "9. Display Current Array\n";
            std::cout << "0. Exit\n";
            std::cout << "Current Array Size: " << size << " | Speed: " << delay << "ms\n";
            std::cout << "Enter choice: ";

            std::cin >> choice;

            switch (choice) {
            case 1:
                generateRandomArray();
                std::cout << "New random array generated!\n";
                break;
            case 2:
                inputCustomArray();
                break;
            case 3:
                bubbleSort();
                break;
            case 4:
                selectionSort();
                break;
            case 5:
                insertionSort();
                break;
            case 6:
                quickSort();
                break;
            case 7: {
                int newSize;
                std::cout << "Enter new array size for random generation (5-50): ";
                std::cin >> newSize;
                if (newSize >= 5 && newSize <= 50) {
                    size = newSize;
                    generateRandomArray();
                    std::cout << "Random array size changed to " << size << "\n";
                }
                else {
                    std::cout << "Invalid size! Please enter between 5 and 50.\n";
                }
                break;
            }
            case 8: {
                int newDelay;
                std::cout << "Enter animation delay in milliseconds (10-1000): ";
                std::cin >> newDelay;
                if (newDelay >= 10 && newDelay <= 1000) {
                    delay = newDelay;
                    std::cout << "Animation speed changed to " << delay << "ms\n";
                }
                else {
                    std::cout << "Invalid delay! Please enter between 10 and 1000.\n";
                }
                break;
            }
            case 9:
                display();
                break;
            case 0:
                std::cout << "Thanks for using Sorting Visualizer!\n";
                break;
            default:
                std::cout << "Invalid choice! Please try again.\n";
            }

            if (choice != 0) {
                std::cout << "Press Enter to continue...";
                std::cin.ignore();
                std::cin.get();
            }

        } while (choice != 0);
    }
};

#ifdef _WIN32
#include <windows.h>

// Fix for MinGW sometimes not defining this constant
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif

int main() {
#ifdef _WIN32
    // Enable virtual terminal processing for ANSI escape codes
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    if (GetConsoleMode(hOut, &dwMode)) {
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
    }

    // Optional: Set terminal to UTF-8 (for symbols, optional)
    system("chcp 65001 > nul");
#endif

    SortingVisualizer visualizer;
    visualizer.menu();
    return 0;
}
