#pragma once

#include <chrono>
#include <iostream>
#include <memory>
#include <string>

#include "kvstore.hpp"

namespace s21 {

class AVLNode {
 public:
  std::string key;
  t_data value;
  unsigned height;
  AVLNode* left;
  AVLNode* right;

  AVLNode(std::string k, t_data val);

  AVLNode* insert(AVLNode* node, std::string inkey, t_data value);
  AVLNode* remove(AVLNode* node, std::string delkey);
  AVLNode* find(AVLNode* node, std::string k);
  void get_keys(AVLNode* node, std::vector<std::string>& ret);
};

}  // namespace s21
