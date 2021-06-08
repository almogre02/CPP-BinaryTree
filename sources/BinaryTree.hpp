#pragma once
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <list>
#include <stack>
#include <queue>

using namespace std;

namespace ariel
{
    template <typename T>
    class BinaryTree
    {
        class Node
        {
        public:
		 	T data;
            Node *left_child=nullptr;
            Node *right_child=nullptr;

            Node(const T& value) : data(value) {}

			void set_data(T value) {this->data = value;}
			T get_data(){return this->data;}
			void delete_node(){
				delete this->left_child;
				delete this->right_child;
			}
			bool is_equal(const Node& check){ return (this->get_data()==check.get_data());}

            Node(const Node& other) : data(other.data)
            {
				if (other.left_child==nullptr){ this->left_child = nullptr;}
				else{ this->left_child = new Node(*other.left_child); }


				if (other.right_child==nullptr){ this->right_child = nullptr;}
				else{ this->right_child = new Node(*other.right_child); }
            }
			Node(const Node&& other) noexcept 
            {
                this->data = other.data;
                this->left_child = other.left_child;
                this->right_child = other.right_child;
             
            }
            Node &operator=(Node&& other) noexcept
            {
                this->data = other.data;
                this->left_child = other.left_child;
                this->right_child = other.right_child;
            }

            Node &operator=(Node other)
            {
                if (this != &other)
                {
					this->data = other.data;
					this->delete_node();

					if (other.left_child==nullptr){ this->left_child = nullptr; }
					else{ this->left_child = new Node(*other.left_child); }
					
					if (other.right_child==nullptr){ this->right_child = nullptr;}
					else{ this->right_child = new Node(*other.right_child); }	  
                }
                return *this;
            }
		
			~Node(){ this->delete_node();}

        };
    
        static void preOrder(vector<Node*>& v,Node** n)
        {
            if (*n != nullptr)
            {
                v.push_back(*n);
                preOrder(v,&(*n)->left_child);
                preOrder(v,&(*n)->right_child);
            }
        }

        static void inOrder(vector<Node*>& v,Node** n)
        {
            if (*n != nullptr)
            {
                inOrder(v,&(*n)->left_child);
                v.push_back(*n);
                inOrder(v,&(*n)->right_child);
            }
        }

        static void postOrder(vector<Node*>& v,Node** n)
        {
            if (*n != nullptr)
            {
                postOrder(v,&(*n)->left_child);
                postOrder(v,&(*n)->right_child);
                v.push_back(*n);
            }
        }


	private:
		Node *root;
    public:
        BinaryTree() : root(nullptr) {}

		Node* find(T value,Node *root) {
        	if(root==nullptr) {
				return nullptr;
            }

        	if(root->data == value) {
				return root;
			}

        	Node* exsist = find(value,root->left_child);

        	if(exsist!=nullptr) {
				return exsist;
			}

        	return find(value,root->right_child); 
    	}

        BinaryTree& add_root(T value)
        {
			if(root==nullptr) {
				root = new Node(value);
			}
        	root->set_data(value);
        	return *this;
        }

        BinaryTree& add_left(const T parent, T left)
        {
			if(root==nullptr) {
				throw invalid_argument{"Error! no nodes in the tree"};
			}
        	Node* exsist = find(parent,root);
        	if (exsist==nullptr) {
				throw invalid_argument{"Error! the parent doesn't exsist"};
			}
        	if (exsist->left_child == nullptr) {
				exsist->left_child = new Node(left); 
				return *this;
			}
        	exsist->left_child->set_data(left);
        	return *this;
        }
        BinaryTree& add_right(const T parent, T right)
        {
			if(root==nullptr) {
				throw invalid_argument{"Error! no nodes in the tree"};
			}
        	Node* exsist = find(parent,root);
        	if (exsist==nullptr) {
				throw invalid_argument{"Error! the parent doesn't exsist"};
			}
        	if (exsist->right_child == nullptr) {
				exsist->right_child = new Node(right); 
				return *this;
			}
        	exsist->right_child->set_data(right);
        	return *this;
        }

        BinaryTree(const BinaryTree& other)
        {
            this->root = new Node(*other.root);
        }
        BinaryTree& operator=(BinaryTree other) noexcept
        {
            if (this != &(other))
            {
                delete (this->root);
                this->root = new Node(*other.root);
            }
            return *this;
        }
        BinaryTree(BinaryTree&& other) noexcept
        {
            this->root = other.root;
        }
        BinaryTree& operator=(BinaryTree&& other) noexcept
        {
            this->root = other.root;
            return *this;
        }

		~BinaryTree() { delete(this->root);}
		
        friend std::ostream& operator<<(std::ostream& os, const BinaryTree<T>& bt)
        {
			os<<"This is my BinaryTree"<<endl;
			os<<*bt;
            return os;
        }

        class Iterator
        {
        private:
            unsigned int it_pos=0;
			unsigned int order_type;
			Node *node_ptr;
            vector<Node*> order_vec;
        public:
            Iterator(Node *ptr = nullptr, unsigned int order = 0): node_ptr(ptr), order_type(order)
            {
                order_vec.clear();

				switch(order_type){  //selects which type of order to use-->preOrder/inOrder/postOrder
					case 1:
						preOrder(order_vec,&node_ptr);
						break;
					case 2:
						inOrder(order_vec,&node_ptr);
						break;
					case 3:
						postOrder(order_vec,&node_ptr);
						break;		
				}
                order_vec.push_back(nullptr);
                node_ptr = order_vec.at(0);
            }

			Iterator operator++(int)  //prefix
            {
                Iterator temp_it = *this;
				this->it_pos=this->it_pos+1;
                this->node_ptr = order_vec.at(it_pos);
                return temp_it;
            }

            Iterator& operator++() //postfix
            {
				this->it_pos=it_pos+1;
                this->node_ptr = order_vec.at(it_pos);
                return *this;
            }

			Iterator& operator--()
			{
				if(node_ptr==nullptr){
					throw("Error! please insert a value before");
				}
				node_ptr=node_ptr->right_child;
				return *this;
			}

			/*should be const because this operator doesnt
            allow changing of the iterator that points to this*/
            T* operator->() const
            {
                return &(node_ptr->data);
            }

			/*should be const because this operator doesnt
            allow changing of the iterator that points to this*/
			T& operator*() const
            {
                return node_ptr->data;
            }

            bool operator==(const Iterator &it) const  
            {
                return (this->node_ptr == it.node_ptr);
            }
			
            bool operator!=(const Iterator &it) const
            {
                return (this->node_ptr != it.node_ptr);
            }

			//only for demo
			friend std::ostream& operator<<(std::ostream& os, const Iterator& it)
        	{
				os<<"This is my BinaryTree"<<endl;
				os<<*it;
            	return os;
        	}

        };

		//*Iterators*//
        Iterator begin_preorder()
        {
            return Iterator{this->root, 1};
        }

        Iterator end_preorder()
        {
            return Iterator(nullptr);
        }

        Iterator begin_inorder()
        {
            return Iterator{this->root, 2};
        }

        Iterator end_inorder()
        {
            return Iterator(nullptr);
        }

        Iterator begin_postorder()
        {
            return Iterator{this->root, 3};
        }

        Iterator end_postorder()
        {
            return Iterator(nullptr);
        }

		Iterator begin()//default in_order
        {
            return begin_inorder();
        }

        Iterator end()
        {
            return Iterator(nullptr);
        }
    };
}
