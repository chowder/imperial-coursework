package utils;

import java.util.Arrays;

public class MinPriorityQueue<T extends Comparable<T>> {

    private static final int capacity = 65536;
    private int queueSize;
    private Object[] queue;


    /**
     * Creates an empty queue.
     */
    public MinPriorityQueue() {
        queue = new Object[capacity];
        queueSize = 0;
        Arrays.fill(queue, null);
    }

    /**
     * Returns the number of elements currently in the queue.
     */
    public int size() {
        return queueSize;
    }
    
    /**
     * Adds elem to the queue.
     */
    public void add(T elem) {
        queue[queueSize] = elem;
        shuffleUp();
        queueSize++;
    }

    /**
     * Removes, and returns, the element at the front of the queue.
     */
    public T remove() {
        T value;
        if (!isEmpty()) {
            value = (T) queue[0];
            queueSize--;
            if (queueSize > 0) {
                queue[0] = queue[queueSize];
                shuffleDown();
            }
        }
        else {
            value = null;
        }
        return value;
    }

    /**
     * Returns true if the queue is empty, false otherwise.
     */
    public boolean isEmpty() {
        return (queueSize == 0);
    }


    private int getParentIndex(int childIndex) {
        return (childIndex - 1) / 2;
    }

    private int getChildIndex(int parentIndex) {
        return parentIndex * 2 + 1;
    }


    private void shuffleUp() {
        int index = queueSize;
        Object temp = queue[index];
        while (compare(queue[getParentIndex(index)], temp) > 0) {
            queue[index] = queue[getParentIndex(index)];
            if (index == 0) {
                break;
            }
            index = getParentIndex(index);
        }
        queue[index] = temp;
    }

    private void shuffleDown(){
        int index = 0;
        Object temp = queue[index];
        while (minChild(index) < queueSize && compare(queue[minChild(index)], temp) < 0) {
            queue[index] = queue[minChild(index)];
            index = minChild(index);
        }
        queue[index] = temp;
    }


    private int minChild(int index) {
        int firstChild = getChildIndex(index);
        int secondChild = firstChild + 1;
        if (secondChild >= queueSize || compare(queue[firstChild], queue[secondChild]) < 0) {
            return firstChild;
        } else {
            return secondChild;
        }
    }


    private int compare(Object one, Object two) {
        return ((T) one).compareTo(((T) two));
    }
}
