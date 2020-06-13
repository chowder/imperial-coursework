package utils;

import java.util.Iterator;
import java.util.NoSuchElementException;

public class List<T> implements Iterable<T>{
    
    public static class ListNode<U> {

        private U           content;
        private ListNode<U> next;

    }

    private ListNode<T> front;
    private ListNode<T> back;
    private int         size;
    
    public List() {
        size  = 0;
    }

    /**
     * Adds elem to the end of the list.
     * @param elem to be added.
     */
    public void add(T elem) {
        ListNode<T> n = new ListNode<T>();
        n.content = elem;
        if (size == 0) {
            front = n;
            back  = n;
        } else {
            back.next = n;
            back      = n;
        }
        size++;
    }

    @Override
    public Iterator<T> iterator() {
        return new Iterator<T>(){

            private ListNode<T> next = front;
            
            @Override
            public boolean hasNext() {
                return next != null;
            }

            @Override
            public T next() {
                if (!hasNext()) { throw new NoSuchElementException(); }
                T elem = next.content;
                next = next.next;
                return elem;
            }

            @Override
            public void remove() {
                throw new UnsupportedOperationException();
            }
            
        };
    }

    public int size() {
        return size;
    }

}


