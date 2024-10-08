#include <stdexcept>

#include "polish_notation/data_structures/node/node.h"
#include "polish_notation/data_structures/queue/queue.h"
#include "polish_notation/utility/utility.h"

namespace polish_notation::data_structures::queue {
using ::polish_notation::data_structures::node::Node;

template <typename T>
void Queue<T>::deleteNodeAndMoveBackPtrToNextNode() noexcept {
    Node<T>* tmp = backPtr_;
    backPtr_ = backPtr_->next;
    delete tmp;
}

template <typename T>
void Queue<T>::copyNodesFrom(const Queue<T>& q) {
    const Node<T>* n = q.backPtr_;

    while (n) {
        enqueue(n->data);
        n = n->next;
    }
}

template <typename T>
inline const Node<T>* Queue<T>::frontPtr() const noexcept {
    return frontPtr_;
}

template <typename T>
inline const Node<T>* Queue<T>::backPtr() const noexcept {
    return backPtr_;
}

template <typename T>
Queue<T>::Queue() noexcept : frontPtr_(), backPtr_(), size_() {}

template <typename T>
Queue<T>::Queue(const Queue& q) : frontPtr_(), backPtr_(), size_() {
    copyNodesFrom(q);
}

template <typename T>
Queue<T> Queue<T>::operator=(const Queue& q) {
    if (this == &q)
        return *this;

    destroy();
    copyNodesFrom(q);

    return *this;
}

template <typename T>
Queue<T>::~Queue() noexcept {
    destroy();
}

template <typename T>
void Queue<T>::enqueue(const T& item) {
    Node<T>* tmp = new Node<T>(item);
    if (isEmpty()) {
        backPtr_ = frontPtr_ = tmp;
    } else {
        frontPtr_->next = tmp;
        frontPtr_ = frontPtr_->next;
    }

    ++size_;
}

template <typename T>
T Queue<T>::dequeue() {
    if (isEmpty())
        throw ::std::runtime_error(::pn_u::debugTrace(
            "runtime_error: Dequeuing from an empty queue."));

    T data = backPtr_->data;

    deleteNodeAndMoveBackPtrToNextNode();

    if (backPtr_ == nullptr)
        frontPtr_ = nullptr;

    --size_;
    return data;
}

template <typename T>
inline T Queue<T>::peek() const {
    if (isEmpty())
        throw ::std::runtime_error(
            ::pn_u::debugTrace("runtime_error: Peeking from an empty queue."));

    return backPtr_->data;
}

template <typename T>
void Queue<T>::destroy() noexcept {
    while (!isEmpty())
        deleteNodeAndMoveBackPtrToNextNode();

    frontPtr_ = backPtr_ = nullptr;
    size_ = 0;
}

template <typename T>
inline bool Queue<T>::isEmpty() const noexcept {
    return backPtr_ == nullptr;
}

template <typename T>
inline ::polish_notation::utility::size_t Queue<T>::size() const noexcept {
    return size_;
}
} // namespace polish_notation::data_structures::queue