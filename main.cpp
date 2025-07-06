#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <Eigen/Dense>
#include <locale>
#include <codecvt>
#include <optional>

using namespace std;
using namespace Eigen;



// 辅助函数：尝试将字符串转换为double
std::optional<double> toDouble(const std::string& str) {
    std::istringstream ss(str);
    double value;
    if (ss >> value && ss.eof()) { // 确保整个字符串都被解析了
        return value;
    }
    return std::nullopt; // 如果不能转换，则返回空
}
// 主函数：读取CSV文件
std::vector<std::vector<double>> readCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    std::vector<std::vector<double>> data;
    std::string line;
    while (std::getline(file, line)) {
        // 移除行首尾的空白字符
        line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));  // 去掉开头空白
        line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);   // 去掉结尾空白
        std::stringstream ss(line);
        std::vector<double> row;
        std::string field;
        while (std::getline(ss, field, ',')) {
            // 移除字段首尾的空白字符
            field.erase(0, field.find_first_not_of(" \t\n\r\f\v"));
            field.erase(field.find_last_not_of(" \t\n\r\f\v") + 1);
            auto value = toDouble(field);
            if (value) {
                row.push_back(*value);
            } else {
                std::cerr << "Warning: Non-numeric value found and skipped: '" << field << "' in line: " << line << std::endl;
            }
        }
        data.push_back(row);
    }
    return data;
}



// 从CSV文件中读取矩阵
MatrixXd readMatrixFromFile(const string& filename) {
    auto data = readCSV(filename);
    int rows = data.size();
    int cols = data[0].size();
    MatrixXd matrix(rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix(i, j) = data[i][j];
        }
    }
    return matrix;
}

// 将矩阵保存到CSV文件
void saveMatrixToFile(const MatrixXd& matrix, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Unable to read: " << filename << endl;
        exit(1);
    }

    int rows = matrix.rows();
    int cols = matrix.cols();
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            file << matrix(i, j);
            if (j < cols - 1) {
                file << ",";
            }
        }
        file << endl;
    }
    file.close();
}

// 计算矩阵的逆
MatrixXd computeInverse(const MatrixXd& matrix) {
    if (matrix.rows() != matrix.cols()) {
        cerr << "The input martix is not square"<<matrix.rows()<< " "<< matrix.cols()  << endl;
        exit(1);
    }

    MatrixXd invMatrix = matrix.inverse();
    if (invMatrix.hasNaN()) {
        cerr << "Unable to inv" << endl;
        exit(1);
    }
    return invMatrix;
}

// 将矩阵输出到控制台
void printMatrix(const MatrixXd& matrix) {
    int rows = matrix.rows();
    int cols = matrix.cols();
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cout << matrix(i, j);
            if (j < cols - 1) {
                cout << " ";
            }
        }
        cout << endl;
    }
}

int main() {
    string inputFilename, outputFilename;
    cout << "Name of file of the input Martix(in csv): ";
    cin >> inputFilename;

    MatrixXd matrix = readMatrixFromFile(inputFilename);
    cout << "Orgin Martix:" << endl;
    printMatrix(matrix);

    MatrixXd invMatrix = computeInverse(matrix);
    cout << "Inv Martix:" << endl;
    printMatrix(invMatrix);

    cout << "Name of output csv file: ";
    cin >> outputFilename;
    saveMatrixToFile(invMatrix, outputFilename);

    cout << "Reverse Martix been written to " << outputFilename << endl;

    return 0;
}
