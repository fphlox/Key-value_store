#include "self_balancing_binary_tree/avl_node.hpp"

namespace s21 {

AVLNode::AVLNode(std::string k, t_data val) {
  key = k;
  value = val;
  left = nullptr;
  right = nullptr;
  height = 1;
}

static unsigned get_height(AVLNode* node) { return node ? node->height : 0; }

static int balance_factor(AVLNode* node) {
  return get_height(node->right) - get_height(node->left);
}

static void height_fixing(AVLNode* node) {
  unsigned h_left = get_height(node->left);
  unsigned h_right = get_height(node->right);
  node->height = (h_left > h_right ? h_left : h_right) + 1;
}

static AVLNode* right_rotate(AVLNode* higer) {
  AVLNode* less = higer->left;
  higer->left = less->right;
  less->right = higer;
  height_fixing(higer);
  height_fixing(less);
  return less;
}

static AVLNode* left_rotate(AVLNode* higer) {
  AVLNode* bigger = higer->right;
  higer->right = bigger->left;
  bigger->left = higer;
  height_fixing(higer);
  height_fixing(bigger);
  return bigger;
}

static AVLNode* balancing(AVLNode* node) {
  height_fixing(node);
  if (balance_factor(node) == 2) {
    if (balance_factor(node->right) < 0)
      node->right = right_rotate(node->right);
    return left_rotate(node);
  }
  if (balance_factor(node) == -2) {
    if (balance_factor(node->left) > 0) node->left = left_rotate(node->left);
    return right_rotate(node);
  }
  return node;
}

AVLNode* AVLNode::insert(AVLNode* node, std::string inkey, t_data value) {
  if (!node) return new AVLNode(inkey, value);
  if (inkey < node->key)
    node->left = insert(node->left, inkey, value);
  else if (inkey > node->key)
    node->right = insert(node->right, inkey, value);
  else {  // inkey = node->key
    return (node);
  }
  return balancing(node);
}

AVLNode* AVLNode::remove(AVLNode* node, std::string delkey) {
  if (!node) return 0;
  if (delkey < node->key)
    node->left = remove(node->left, delkey);
  else if (delkey > node->key)
    node->right = remove(node->right, delkey);
  else  //  delkey == node->key
  {
    delete node;
    node = nullptr;
    return node;
  }
  return balancing(node);
}

AVLNode* AVLNode::find(AVLNode* node, std::string k) {
  if (!node) return 0;
  if (k < node->key)
    return find(node->left, k);
  else if (k > node->key)
    return find(node->right, k);
  return node;
}

void AVLNode::get_keys(AVLNode* node, std::vector<std::string>& ret) {
  if (!node) return;
  if (node->left) get_keys(node->left, std::ref(ret));
  if (node->right) get_keys(node->right, std::ref(ret));
  if (node) {
    ret.push_back(node->key);
  }
  return;
}

}  // namespace s21
