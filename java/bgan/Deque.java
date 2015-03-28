import java.util.Iterator;
import java.util.NoSuchElementException;

/**
 * @author brlv
 *
 * generalization of a stack and a queue that
 * supports inserting and removing items from
 * either the front or the back of the data structure
 *
 * @param <Item> item type
 */
public class Deque<Item> implements Iterable<Item> {

    private class Node {
        private Item item;
        private Node prev;
        private Node next;
    }
    private int size;
    private Node first;
    private Node last;
    
    public Deque() {
         first = null;
         last = null;
         size = 0;
     }

    private class ListIterator implements Iterator<Item> {

        private Node current = first;

        @Override
        public boolean hasNext() { return current != null; }

        @Override
        public void remove() { throw new UnsupportedOperationException(); }

        @Override
        public Item next() {
            if (!hasNext()) {
                throw new NoSuchElementException();
            }
            Item item = current.item;
            current = current.next;
            return item;
        }
    }

    @Override
    public Iterator<Item> iterator()  { return new ListIterator(); }

    public boolean isEmpty() { return size == 0; }

    public int size() { return size; }

    public void addFirst(Item item) {
        if (item == null) {
            throw new java.lang.NullPointerException();
        }

        Node oldfirst = first;

        first = new Node();
        first.item = item;
        first.prev = null;
        first.next = oldfirst;

        if (isEmpty()) {
            last = first;
        } else {
            oldfirst.prev = first;
        }

        size++;
    }

    public void addLast(Item item) {
        if (item == null) {
            throw new java.lang.NullPointerException();
        }
        Node oldlast = last;

        last = new Node();
        last.item = item;
        last.prev = oldlast;
        last.next = null;

        if (isEmpty()) {
            first = last;
        } else {
            oldlast.next = last;
        }

        size++;
    }

    public Item removeFirst() {
        if (isEmpty()) {
            throw new java.util.NoSuchElementException();
        }

        Item item = first.item;

        first = first.next;
        if (first != null) { // to avoid loitering
            first.prev = null;
        }
        size--;
        if (isEmpty()) {
            last = null;
        }

        return item;
    }

    public Item removeLast() {
        if (isEmpty()) {
            throw new java.util.NoSuchElementException();
        }

        Item item = last.item;

        last = last.prev;
        if (last != null) { // to avoid loitering
            last.next = null;
        }
        size--;
        if (isEmpty()) {
            first = null;
        }

        return item;
    }
}
