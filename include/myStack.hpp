#pragma once

#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <utility>
#include "utils.hpp"


namespace handMadeStack
{

	template <typename Data_t>
	class myStack
	{
	public:

		myStack();

		myStack(size_t size);

		// Copy/move constructor
		myStack(const myStack& stack);
        myStack(myStack&& stack);

        // Copy/move assignment
        myStack& operator=(const myStack& Stack);
        myStack& operator=(myStack&& Stack);

        // Destructor
        ~myStack();

        size_t size() const;
        size_t capacity() const;
        myStack& resize(size_t size);


        void push(const Data_t& element);
        void push(Data_t&& element);
        
        void pop();

        Data_t& top() const;

        bool empty() const;

        bool operator==(const myStack& other) const;
        bool operator!=(const myStack& other) const;

        bool good() const;


        template <typename V_t>
        void emplace(V_t val)
        {
            ASSERT(this->good(), "Invalid stack\n");

            if (size_ == capacity_)
            {
                this->resize(capacity_*2 + 1);
            }

            new (data_ + size_) Data_t(val);
            size_++;

            ASSERT(this->good(), "Unable to emplace element\n");
        }

        template<typename V_t, typename... Args>
        void emplace(V_t val, Args... args)
        {
            emplace(val);

            emplace(args...) ;
        }


	private:

		Data_t* data_;
		size_t size_;
		size_t capacity_;


	}; // class myStack


	template <typename Data_t>
	myStack<Data_t>::myStack():
        data_ (nullptr),
        size_ (0),
        capacity_ (0)
	{
		ASSERT(this->good(), "Unable to construct stack\n");
	}


	template <typename Data_t>
	myStack<Data_t>::myStack(size_t size):
        data_ (new Data_t[size]),
        size_ (0),
        capacity_ (size)
	{
        if (capacity_ == 0) {delete[] data_; data_ = nullptr;}
		ASSERT(this->good(), "Unable to construct stack\n");
	}


	template <typename Data_t>
	myStack<Data_t>::myStack(const myStack<Data_t>& stack)
	{
    	ASSERT(stack.good(), "Origin stack is invalid\n");

    	data_     = new Data_t[stack.size_];
    	size_     = stack.size_;
    	capacity_ = stack.capacity_;

    	std::copy_n(stack.data_, size_, data_);

    	ASSERT(this->good(), "Unable to construct stack\n");
	}


    template <typename Data_t>
	myStack<Data_t>::myStack(myStack<Data_t>&& stack)
    {
        ASSERT(stack.good(), "Unable to create stack from invalid origin\n");
        
        data_     = stack.data_;
        size_     = stack.size_;
        capacity_ = stack.capacity_;

        stack.data_ = nullptr;
        stack.size_ = 0U;
        stack.capacity_ = 0U;

        ASSERT(this->good(), "Unable to construct stack\n");

        ASSERT(stack.empty() && (stack.data_ == nullptr), "Origin was not destructed after move\n");
    }


    template <typename Data_t>
    myStack<Data_t>& myStack<Data_t>::operator=(const myStack<Data_t>& stack)
    {
        ASSERT(this->good(), "Left operand of copy assignment is invalid\n");
        ASSERT(stack.good(), "Right operand of copy assignment is invalid\n");

        // If left operand is a right operand at same time
        if (this == &stack) {return *this;}

        delete[] data_;

        data_     = new Data_t[stack.size_];
        size_     = stack.size_;
        capacity_ = stack.capacity_;

        std::copy_n(stack.data_, size_, data_);

        ASSERT(this->good(), "Unable to copy stack\n");

        return *this;
    }


    template <typename Data_t>
    myStack<Data_t>& myStack<Data_t>::operator=(myStack<Data_t>&& stack)
    {
        ASSERT(this->good(),
            "Left operand of move assignment is invalid\n");
        ASSERT(stack.good(),
            "Right operand of move assignment is invalid\n");
        ASSERT(this != &stack,
            "Move assignment from itself must not be generated by compiler. How?\n");

        delete[] data_;

        data_     = stack.data_;
        size_     = stack.size_;
        capacity_ = stack.capacity_;

        stack.data_ = nullptr;
        stack.size_ = 0U;
        stack.capacity_ = 0U;

        ASSERT(this->good(), "Unable to perform move assignment\n");

        ASSERT(stack.empty() && (stack.data_ == nullptr), "Origin was not destructed after move\n");

        return *this;
    }

    template <typename Data_t>
    myStack<Data_t>::~myStack()
    {
        if (data_ != nullptr)
        {
            delete[] data_;
        }

        data_ = nullptr;
    }

    //------------------
    // Interface && other
    //------------------

    template <typename Data_t>
    size_t myStack<Data_t>::size() const
    {
        ASSERT(this->good(), "Unable to perform move assignment\n");

        return size_;
    }

    template <typename Data_t>
    size_t myStack<Data_t>::capacity() const
    {
        ASSERT(this->good(), "Unable to perform move assignment\n");

        return capacity_;
    }

    template <typename Data_t>
    myStack<Data_t>& myStack<Data_t>::resize(size_t new_size)
    {
        ASSERT(this->good(), "Invalid stack\n");

        if (new_size == size_) {return *this;}

        Data_t* new_data = (new_size > 0) ? (new Data_t[new_size]) : (nullptr);

        //std::copy_n(data_, std::min(new_size, size_), new_data);
        my_copy_n(data_, std::min(new_size, size_), new_data);

        delete[] data_;

        data_ = new_data;
        size_ = std::min(new_size, size_);
        capacity_ = new_size;

        ASSERT(this->good(), "Unable to resize\n");

        return *this;
    }

    template <typename Data_t>
    void myStack<Data_t>::push(const Data_t& element)
    {
        ASSERT(this->good(), "Invalid stack\n");

        if (size_ == capacity_)
        {
            this->resize(capacity_*2 + 1);
        }

        data_[size_] = element;
        size_++;

        ASSERT(this->good(), "Unable to push element\n");
    }

    //////////////////////////////////////////////////////////////////////////////
    template <typename Data_t>
    void myStack<Data_t>::push(Data_t&& element)
    {
        ASSERT(this->good(), "Invalid stack\n");

        if (size_ == capacity_)
        {
            this->resize(capacity_*2 + 1);
        }

        data_[size_] = std::move(element);
        size_++;

        ASSERT(this->good(), "Unable to push element\n");
    }
    //////////////////////////////////////////////////////////////////////////////

    template <typename Data_t>
    void myStack<Data_t>::pop()
    {
        ASSERT(this->good(), "Invalid stack\n");

        ASSERT(!this->empty(), "Unable to pop from empty stack\n");

        // Call destructor of element
        data_[size_-1].~Data_t();
        size_--;

        ASSERT(this->good(), "Unable to push element\n");
    }

    template <typename Data_t>
    Data_t& myStack<Data_t>::top() const
    {
        ASSERT(this->good(), "Invalid stack\n");

        ASSERT(!this->empty(), "Unable to get top of empty stack\n");

        return data_[size_-1];
    }

    template <typename Data_t>
    bool myStack<Data_t>::empty() const
    {
        ASSERT(this->good(), "Invalid stack\n");

        return (size_ == 0);
    }


    template <typename Data_t>
    bool myStack<Data_t>::operator==(const myStack<Data_t>& stack) const
    {
        ASSERT(this->good(),
            "Left operand of move assignment is invalid\n");
        ASSERT(stack.good(),
            "Right operand of move assignment is invalid\n");

        if (this == &stack) {return true;}

        if (size_ != stack.size()) {return false;}

        for (int i = 0; i < static_cast<int>(size_); i++)
        {
            if (data_[i] != stack.data_[i])
            {
                return false;
            }
        }

        return true;
    }

    template <typename Data_t>
    bool myStack<Data_t>::operator!=(const myStack<Data_t>& stack) const
    {
        return !((*this)==stack);
    }

    template <typename Data_t>
    bool myStack<Data_t>::good() const
    {
        return (capacity_ >= size_) && ((capacity_ > 0) ^ (data_ == nullptr));
    }

} // namespace handMadeStack

/*
1) PUSH & and PUSH &&
1)push back
2)pop back
*/