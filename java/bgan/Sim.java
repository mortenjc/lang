import static org.junit.Assert.*;

public class Sim {
  private MinPQ<Event> pq;                  // event (priority) queue
  private ResizingArrayQueue<Packet> iq;    // input queue for terminal
  private Packet iqSpill;                   // holds a fragment (terminal)
  private ResizingArrayQueue<Packet> iqran; // input queue for RAN
  private Packet iqranSpill;                // holds fragment (RAN) 
  
  private double t     = 0.0;               // simulation clock time, t is in us
  private int timeSlot = 0;                 // Timeslot identifier
  
  private final double delay = 240000;      // 240 ms delay for air interface
  
  Sim() {
    pq    = new MinPQ<Event>();
    iq    = new ResizingArrayQueue<Packet>();
    iqran = new ResizingArrayQueue<Packet>();
  }
  
  public void simulate() { 
    int packetno = 1;
    
    // Add first 20ms schedule (third Event argument is slot capacity) 
    pq.insert(new Event(20000, Event.E_20MS, 1024, timeSlot++));
    
    // Add incoming packets at 256000 bits/s
    double dt = 1000000/(256000/(8*512));
    for (int i = 1; i < 20; i++)
      pq.insert(new Event(i*dt, Event.E_NEWPKT, 512, -1));
  
    // Main event loop, stop after 1s of simulation
    System.out.println("---------------------------------------------------------------------"); 
    System.out.println("time (ms)       terminal input    terminal output    RAN input");
    System.out.println("---------------------------------------------------------------------");

    while (!pq.isEmpty() && (t < 1000000.0)) { 
      
      Event e = pq.delMin();
      t = e.time;
       
      if (e.type == Event.E_20MS) {            // Handle RAN 20 ms slots
        handle20ms(e);
      } else if (e.type == Event.E_NEWPKT) {   // New packet arrives at terminal
        iq.enqueue(new Packet(packetno, e.data, t, Packet.WHOLE));
        printPacket(packetno, 512, true);
        packetno++;
      } else if (e.type == Event.E_Q2) {       // Packet arrives at RAN
        handleRanRx(e);
      } else {
        System.out.println("Illegal event");
        throw new IllegalArgumentException();
      }
    }
  }
  //---------------------------------
  
  private void printPacket(int seq, int len, boolean newline) {
    StdOut.printf("%8.3f", t/1000);
    System.out.print(" ms :   pkt " + seq + " (" + len +")");  
    if (newline)
      System.out.println();
  }
  
  /**  
   *  Function called when packets arrive at RAN
   */
  private void handleRanRx(Event e) {
    Packet pkt = iqran.dequeue();
    double delay = t - pkt.timeus;
    if (pkt.flags == Packet.WHOLE) {         // A whole packet is ready
      StdOut.printf("%8.3f", t/1000);
      System.out.print(" ms :                                       pkt " + pkt.seqno + " ("+ pkt.size+ ")");
      StdOut.printf("%8.3f\n", delay/1000);
    } else if (pkt.flags == Packet.START) {  // Part 1/2 of a packet - save
      iqranSpill = pkt;
    } else if (pkt.flags == Packet.END) {    // Part 2/2 of a packet - reassemble
      assertEquals("consistency", true, iqranSpill.seqno == pkt.seqno);
        
      int len = iqranSpill.size + pkt.size;
      
      StdOut.printf("%8.3f", t/1000);
      System.out.print(" ms :                                       pkt " + pkt.seqno + " ("+ len+ ") reass.");
      StdOut.printf("%8.3f\n", delay/1000);
    }  
  }
  
  /**  
   *  Function called every 20ms to transmit packets from Terminal to RAN
   */
  private void handle20ms(Event e) {
    pq.insert(new Event(t + 20000.0, Event.E_20MS, 1020, timeSlot++));
    double newtime = t + delay;
    
    int bytes2Send = e.data;
    int bytes = 0;
    
    if (bytes2Send == 0)
      return;
   
    // Always start with packet in the spill buffer
    if (iqSpill != null) {
      if (iqSpill.size > bytes2Send)
        return;
      
      iqran.enqueue(iqSpill);
      bytes = iqSpill.size;
      
      StdOut.printf("%8.3f", t/1000);
      System.out.println(" ms :                     pkt "+ iqSpill.seqno+" (" + iqSpill.size +") frag.");
      iqSpill = null;
      pq.insert(new Event(newtime, Event.E_Q2, 0, -1));
    }
     
    while (bytes < bytes2Send) {      
      if (!iq.isEmpty()) {
        Packet pkt = iq.dequeue();
        
        // If room for a full packet - do the usual
        if (pkt.size < (bytes2Send - bytes)) {
          iqran.enqueue(pkt);
          bytes+= pkt.size;
          StdOut.printf("%8.3f", t/1000);
          System.out.println(" ms :                     pkt "+ pkt.seqno+" (" + pkt.size +")");
          pq.insert(new Event(newtime, Event.E_Q2, 0, -1));
        } else {
          
          // Only room for part of a packet - split
          Packet a = new Packet(pkt.seqno, (bytes2Send - bytes), pkt.timeus, Packet.START);
          iqran.enqueue(a);
          StdOut.printf("%8.3f", t/1000);
          System.out.println(" ms :                     pkt "+ a.seqno+" (" + a.size +") frag.");
          pq.insert(new Event(newtime, Event.E_Q2, 0, -1));
          
          iqSpill = new Packet(pkt.seqno, pkt.size - (bytes2Send -bytes), pkt.timeus, Packet.END);
        }
      } else  // nothing in queue - should this be an error?
        return;
    }   
  }

  // -------------------------------------------------------------
  
  // Helper classes
  
  private static class Packet {
    private static final int START = 0x04;
    private static final int END   = 0x08;
    private static final int WHOLE = START + END;
    private int seqno;
    private int size;
    private int flags;
    private double timeus;
    
    Packet(int seq, int len, double time, int flag) {
      seqno = seq;
      size = len;
      flags = flag;
      timeus = time;
    }
  }
  
  private static class Event implements Comparable<Event> {
    private final double time;  // time that event is scheduled to occur
    private final int type;     // E_* events defined below
    private final int data;     // RAN capacity, Packet size
    private final int data2;    // Timeslot # RAN
    
    private static final int E_20MS   = 0;
    private static final int E_NEWPKT = 1;
    private static final int E_Q2     = 2;
    
    // create a new event to occur at time t involving
    public Event(double t, int type, int data, int data2) {
      this.time  = t;
      this.type  = type;
      this.data  = data;
      this.data2 = data2;
    }
    
    // compare times when two events will occur
    public int compareTo(Event that) {
      if      (this.time < that.time) return -1;
      else if (this.time > that.time) return +1;
      else                            return  0;
    }
  }
  
  public static void main(String [] args) {
    Sim bgan = new Sim();
    bgan.simulate();
  }
  
}