#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>


std::vector<int> str_to_vec(std::string str) {
  std::vector<int> result(str.size());
  for (size_t i = 0; i < str.size(); i++) {
    result.at(i) = str.at(i) - '0';
  }
  return result;
}

std::string vec_to_str(std::vector<int> vec) {
  std::string result;
  for (int i : vec) {
    result += std::to_string(i);
  }
  return result;
}

std::vector<int> addition(std::vector<int> num1, std::vector<int> num2, int B) {
  // the least significant bit is at the right (highest index)
  std::vector<int> result;
  int carry = 0;
  auto n1 = num1.size();
  auto n2 = num2.size();
  auto max_size = std::max(n1, n2);
  // loop runs while i < max_size or while carry is non-zero
  for (size_t i = 0; i < max_size || carry; i++) {
    int digit1;
    if (i < n1) {
      digit1 = num1.at(n1 - i - 1);
    } else {
      digit1 = 0;
    }
    int digit2;
    if (i < n2) {
      digit2 = num2.at(n2 - i - 1);
    } else {
      digit2 = 0;
    }
    int sum = digit1 + digit2 + carry;
    result.push_back(sum % B);
    carry = sum / B;
  }
  // reverses the result vector as the push_back made the LSB at index 0
  // index, so needs to be reversed to put MSB at index 0
  std::reverse(result.begin(), result.end());
  return result;
}

std::vector<int> subtraction(const std::vector<int>& num1, const std::vector<int>& num2, int B) {
  std::vector<int> result;
  int borrow = 0;
  size_t n1 = num1.size();
  size_t n2 = num2.size();

  for (size_t i = 0; i < n1; ++i) {
    int digit1 = num1.at(n1 - 1 - i);
    int digit2;
    if (i < n2) {
      digit2 = num2.at(n1 - 1 - i);
    } else {
      digit2 = 0;
    }
    int diff = digit1 - digit2 - borrow;
    if (diff < 0) {
      diff = diff + B;
      borrow = 1;
    } else {
      borrow = 0;
    }
    result.push_back(diff);
  }

  while (result.size() > 1 && result.back() == 0) result.pop_back();
  std::reverse(result.begin(), result.end());
  return result;
}

std::vector<int> multiplication(std::vector<int> num1, std::vector<int> num2, int B) {
  std::vector<int> result;
  std::reverse(num1.begin(), num1.end());
  std::reverse(num2.begin(), num2.end());

  for (int i = 0; i < num2.size(); i++) {
    int carry = 0;
    std::vector<int> temp(i, 0);
    for (int j = 0; j < num1.size(); j++) {
      int product = num1.at(j) * num2.at(i) + carry;
      temp.push_back(product % B);
      carry = product / B;
    }

    if (carry != 0) {
      temp.push_back(carry);
    }

    std::reverse(temp.begin(), temp.end());
    result = addition(result, temp, B);
  }

  return result;
}

std::vector<int> karatsuba(std::vector<int> num1, std::vector<int> num2, int B) {
 
  int n = std::max(num1.size(), num2.size());
  if (n == 3) {
    return multiplication(num1, num2, B); // school multiplication is n is smaller than 4
  }
  int mid = ceil(mid / 2);

  std::vector<int> a1(num1.begin(), num1.begin() + mid);
  std::vector<int> a0(num1.begin() + mid, num1.end());
  std::vector<int> b1(num2.begin(), num2.begin() + mid);
  std::vector<int> b0(num2.begin() + mid, num2.end());

  std::vector<int> a0b0 = karatsuba(a0, b0, B);  // (a0 * b0)
  std::vector<int> a1b1 = karatsuba(a1, b1, B);  // (a1 * b1)
  // (a1 + a0) * (b1 + b0)
  std::vector<int> P0 = karatsuba(addition(a1, a0, B), addition(b1, b0, B), B);

  // (a1 + a0) * (b1 + b0) - (a1 * b1 + a0 * b0)
  std::vector<int> P1 = subtraction(P0, addition(a0b0, a1b1, B), B);
 
  std::vector<int> ans(n * 2, 0); // final vector is 2 times the length
    for (size_t i = 0; i < a1b1.size(); i++) {
      ans.at(i + n) += a0b0.at(i); // placing a0b0 at the end of the vector (least significant)
      ans.at(i + mid) += P1.at(i); // placing P1 in middle starting at mid to "multiply by B^k"
      ans.at(i) += a1b1.at(i); // placing a1b1 at MSB to "multiply by b^2k"
  }

  return ans;
}


int main() {
  std::string I1, I2;
  int B;
  std::cin >> I1 >> I2 >> B;

  std::vector<int> num1 = str_to_vec(I1);
  std::vector<int> num2 = str_to_vec(I2);

  std::vector<int> sum = addition(num1, num2, B);
  std::cout << vec_to_str(sum) << " ";

  std::vector<int> product = karatsuba(num1, num2, B);
  std::cout << vec_to_str(product) << " 0" << std::endl;
  return 0;
}