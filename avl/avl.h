#ifndef AVL_HEAD_H_2022_03_24
#define AVL_HEAD_H_2022_03_24

#include <algorithm>
#include "mm.h"

namespace templates
{

    template <class T, int(*Compare)(const T *pElement, const T* pElement2) >
    class CAVLTree
    {
    private:

        const int BLOCK_SIZE = 10;

        struct leaf
        {
            T* pData;
            leaf *pLeft;
            leaf *pRight;
            int balanceFactor;

            leaf() = default;
        };

    public:
        class CException
        {
        public:
            CException()
            {
            }
        };

    public:
        CAVLTree() : m_pRoot(nullptr), m_Memory(lab618::CMemoryManager<leaf>(BLOCK_SIZE, true))
        {
        }

        virtual ~CAVLTree()
        {
            clear();
        }

        bool add(T* pElement)
        {
            if (findLeaf(pElement) == nullptr) {
                return insert(*m_pRoot, pElement) != nullptr;
            }
            return false;
        }


        bool update(T* pElement)
        {
            leaf* tmpLeaf = findLeaf(pElement);
            if (tmpLeaf) {
                tmpLeaf->pData = pElement;
                return true;
            }
            insert(m_pRoot, pElement);
            return false;
        }


        T* find(const T& pElement)
        {
            leaf* tmp = findLeaf(&pElement);
            if (tmp == nullptr) {
                return nullptr;
            }
            return tmp->pData;
        }


        bool remove(const T& element)
        {
            leaf* tmp = findLeaf(& element);
            if (tmp == nullptr) {
                return false;
            }
            return remove_leaf(tmp) != nullptr;
        }

        void clear()
        {
            m_Memory.clear();
        }

    private:


//        void childrenBF(leaf* root, int& left_child_BF, int& right_child_BF) {
//            if (root->pLeft != nullptr) {
//                left_child_BF = root->pLeft->balanceFactor;
//            }
//            if (root->pRight != nullptr) {
//                right_child_BF = root->pRight->balanceFactor;
//            }
//        }



//
//
//        leaf* recFind(leaf* root, const T& element) {
//            if (nullptr == root) {
//                return nullptr;
//            }
//            int comp_result = Compare(root->pData, &element);
//            if (comp_result > 0) {
//                return recFind(root->pRight, element);
//            }
//            if (comp_result < 0) {
//                return recFind(root->pLeft, element);
//            }
//            return root;
//        }


    private:
        leaf* m_pRoot;
        lab618::CMemoryManager<leaf> m_Memory;

        void updateBF(leaf* root, int left_height, int right_height) {
            root->balanceFactor = left_height - right_height;
        }

        leaf* balance(leaf* root) {
            int curr_BF = root->balanceFactor;
            int left_child_BF = 0;
            int right_child_BF = 0;
            if (root->pLeft != nullptr) {
                left_child_BF = root->pLeft->balanceFactor;
            }
            if (root->pRight != nullptr) {
                right_child_BF = root->pRight->balanceFactor;
            }
            if (curr_BF == -2) {
                if (right_child_BF > 0) {
                    root->pRight = smallRotateRight(root->pRight);
                }
                return smallRotateLeft(root);
            }
            if (curr_BF == 2) {
                if (left_child_BF < 0) {
                    root->pLeft = smallRotateLeft(root->pLeft);
                }
                return smallRotateRight(root);
            }
            return root;
        }

        leaf* smallRotateLeft(leaf* p) {
            leaf* q = p->pRight;
            p->pRight = q->pLeft;
            q->pLeft = p;
            int left_child_BF_q = 0;
            int right_child_BF_q = left_child_BF_q - q->balanceFactor;
            int left_child_BF_p = std::max(left_child_BF_q, right_child_BF_q) + 1 + p->balanceFactor;
            updateBF(p, left_child_BF_p, left_child_BF_q);
            updateBF(q, std::max(left_child_BF_p, left_child_BF_q) + 1, right_child_BF_q);
            return q;
        }

        leaf* smallRotateRight(leaf* p) {
            leaf* q = p->pLeft;
            p->pLeft = q->pRight;
            q->pRight = p;
            int left_child_BF_q = 0;
            int right_child_BF_q = left_child_BF_q - q->balanceFactor;
            int right_child_BF_p = std::max(left_child_BF_q, right_child_BF_q) + 1 - p->balanceFactor;
            updateBF(p, right_child_BF_q, right_child_BF_p);
            updateBF(q, left_child_BF_q, std::max(right_child_BF_p, right_child_BF_q) + 1);
            return q;
        }

        leaf* findLeaf(const T* pElement) {
            leaf* curr = m_pRoot;
            while (curr) {
                int is_equal = Compare(pElement, curr->pData);
                if (is_equal == 0) {
                    return curr;
                }
                if (is_equal < 0) {
                    curr = curr->pLeft;
                } else {
                    curr = curr->pRight;
                }
            }
            return nullptr;
        }

        leaf* recFindMin(leaf* root) {
            if (nullptr == root->pLeft) {
                return root;
            }
            return recFindMin(root->pLeft);
        }

        leaf* recRemoveMin(leaf* root) {
            if (nullptr == root->pLeft) {
                return root->pRight;
            }
            root->pLeft = recRemoveMin(root->pLeft);
            root = balance(root);
            return root;
        }

        leaf* remove_element(leaf* root, const T& element, bool& is_success) {
            if (nullptr == root) {
                return nullptr;
            }
            int comp_result = Compare(root->pData, &element);
            if (comp_result > 0) {
                root->pRight = remove_element(root->pRight, element, is_success);
            }
            else if (comp_result < 0) {
                root->pLeft = remove_element(root->pLeft, element, is_success);
            } else {
                leaf* left_child = root->pLeft;
                leaf* right_child = root->pRight;
                m_Memory.deleteObject(root);
                is_success = true;
                if (nullptr == right_child) {
                    return left_child;
                }

                leaf* min_leaf_in_right = recFindMin(right_child);
                min_leaf_in_right->pRight = recRemoveMin(right_child);
                min_leaf_in_right->pLeft = left_child;
                return balance(min_leaf_in_right);
            }
            return balance(root);
        }

        leaf* insert(leaf* root, T* pElement) {
            if (root == nullptr) {
                leaf* new_leaf = m_Memory.newObject();
                new_leaf->pData = pElement;
                root = new_leaf;
                return new_leaf;
            }
            int comp_result = Compare(root->pData, pElement);
            if (comp_result < 0) {
                root->pLeft = insert(root->pLeft, pElement);
                if (root->pLeft != nullptr) {
                    ++root->balanceFactor;
                }
            } else {
                root->pRight = insert(root->pRight, pElement);
                if (root->pRight != nullptr) {
                    --root->balanceFactor;
                }
            }
            root = balance(root);
            return root;
        }
    };

}; // namespace templates

#endif //AVL_HEAD_H_2022_03_24
