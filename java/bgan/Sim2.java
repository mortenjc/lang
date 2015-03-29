import static org.junit.Assert.*;
import java.util.Random;

public class Sim2 {
  private MinPQ<Event> pq;    // event (priority) queue
  private Deque<Packet> dp;   // 
  private int N;              // number of packets to transmit
  private int packetsize;     // size of each packet
  ResizingArrayQueue<Packet> rq;

  // Stats only
  Histogram H;
  
  // System constants
  private static final double t0            = 0.1235731; // Just a small number
  private static final int maxCapacity      =   1232;    // Max is 1232 ??
  private static final int maxQueuedPackets =     12;    // Completely arbitrary for now
  private static final double airDelay      = 252187;    // propagation delay for air interface
  
  public enum EventType {
      FIRST_IN_TERMINAL, LAST_IN_TERMINAL, RAN_SCHEDULE, FIRST_IN_RAN, LAST_IN_RAN
  }
  
  private static final int _100Mbps_ = 100000000;
  private static final int _492kbps_ =    492000;
  
  private double t     = t0; // simulation clock time, t is in us
  private int timeSlot =  1;
  
  Sim2(int numPackets, int size) {
    N  = numPackets;
    packetsize = size;
    pq = new MinPQ<Event>();
    dp = new Deque<Packet>();
    rq = new ResizingArrayQueue<Packet>();
    H  = new Histogram(N);
  }
  
  /** return transmission time in us for N bytes at given linkspeed */
  private double delayUs(int bytes, int linkspeed) {
    return (bytes*8.0/linkspeed)*1000000.0;
  }
  
  public void simulate() { 
    // incoming packets rate at 256000 bits/s
    double dt = 1000000.0/(256000.0/(8*packetsize));
    Random rand = new Random();
    
    // Add initial 20ms schedule at t0 + 20ms
    pq.insert(new Event(t + 20000.0, EventType.RAN_SCHEDULE, maxCapacity, timeSlot++, null));
    
    // Schedule all packets for transmission
    for (int i = 1; i <= N; i++) {
      double time = i*dt;
      pq.insert(new Event(time, EventType.FIRST_IN_TERMINAL, packetsize, i, null));
    }
    
    // Main event loop
    double deltat = t;
    boolean stop  = false;
    while (!pq.isEmpty() && (stop == false) ) { 

      Event e = pq.delMin();
      t = e.time;
      
      if ((t - deltat) >= 10000000.0) {
        System.out.println("Time: "+ t/1000000 + " s");
        deltat = t;
      }
      
      switch(e.type) {
        case FIRST_IN_TERMINAL:
          int seqNo = e.data2;
          Packet pkt = new Packet(seqNo, e.data, t, Packet.WHOLE);
          pq.insert(new Event(t + delayUs(e.data, _100Mbps_), EventType.LAST_IN_TERMINAL, -1, -1, pkt));
          //System.out.printf("Packet %3d first bit in at terminal: %4.3fms\n", seqNo, t/1000);
          break;

        case LAST_IN_TERMINAL:
          Packet p = e.p;
          if (dp.size() >= maxQueuedPackets) {
            System.out.printf("RxQueue Full - dropping packet %d\n", p.seqno);
          } else {
            dp.addFirst(p);
            //System.out.printf("Packet %3d last  bit in at terminal: %4.3fms\n", p.seqno, t/1000);
          }
          break;          
          
        case RAN_SCHEDULE:
          //System.out.print("---------------------------------------");
          //System.out.printf("%5.4f\n", e.time/1000);
          handle20ms(e);
          break;

        case FIRST_IN_RAN: // Add delay until last bit is in
          Packet p2 = e.p;
          //System.out.printf("Packet %3d first bit in at RAN     : %4.3fms\n", p2.seqno, t/1000);
          pq.insert(new Event(t + delayUs(p2.size, _492kbps_), EventType.LAST_IN_RAN, -1, -1, p2));
          break;
          
        case LAST_IN_RAN:
          handleRanRx(e);
          if (e.p.seqno == (N-1)) {
            //System.out.println("Stopping after " + e.eventno + " events");
            stop = true;
            //H.histogram(20);
            H.stats();
          }
          break;
          
        default:
          System.out.println("Illegal event");
          throw new IllegalArgumentException();
      }  
    }
  }

  private void handleRanRx(Event e) {
    Packet p = e.p;
    double delay = (e.time - p.timeus - airDelay)/1000;

    if (p.flags == (Packet.WHOLE)) {
      //System.out.printf("Packet %3d last  bit in at RAN     : %4.3fms (delay %4.3f)\n", p.seqno, t/1000, delay);  
      H.add(delay);
    } else if (p.flags == Packet.START) {
      //System.out.printf("START frag of packet %d (%3d bytes): %4.3f\n", p.seqno, p.size, e.time/1000); 
    } else if (p.flags == Packet.END) {
      //System.out.printf("Packet %3d (f) last bit in at RAN  : %4.3fms (delay %4.3f)\n", p.seqno, e.time/1000, delay); 
      H.add(delay);
    } else
      throw new IllegalArgumentException("Implementation Error");   
  }
  
  /**  
   *  Function called every 20ms to transmit packets from Terminal to RAN
   */
  private void handle20ms(Event e) {    
    pq.insert(new Event(t + 20000, EventType.RAN_SCHEDULE, maxCapacity, -1, null));
    int capacity = e.data;
    
    // First build queue of packets to transmit
    
    int remaining = capacity;
    int tosend    = 0;
    int packetstosend = 0;
    int qsize= dp.size();
    while (!dp.isEmpty() && remaining > 0) {
     
      Packet p = dp.removeLast();
      packetstosend++;
      if (p.size <= remaining) { // Room for whole packet
        tosend+= p.size;
        remaining-= p.size;
        rq.enqueue(p);    
      } else {  // Only room for a fragment - SPLIT!
        //if (p.flags != Packet.WHOLE) {
        //  System.out.printf("DOUBLE Split %d\n", p.seqno);
        //}
        Packet a = new Packet(p.seqno, remaining,       p.timeus, Packet.START);
        tosend+= remaining;
        remaining = 0;
        rq.enqueue(a);
        
        Packet b = new Packet(p.seqno, p.size - a.size, p.timeus, Packet.END);
        dp.addLast(b); // Push back on queue
      }
    }
    
    assertEquals("Queue check", qsize, packetstosend);
    
    // Next transmit them sequentially
    double delay = 0.0;
    while (!rq.isEmpty()) {
      Packet p = rq.dequeue();
      //System.out.printf("Transmitting packet %d (%d) %4.3f\n", p.seqno, p.size, e.time/1000);
      pq.insert(new Event(t + airDelay + delay, EventType.FIRST_IN_RAN, -1, -1, p));
      delay += delayUs(p.size, _492kbps_);
    }
    assertEquals("local queue is empty", true, rq.isEmpty());
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
      seqno  = seq;
      size   = len;
      flags  = flag;
      timeus = time;
    }
  }
  
  private static class Event implements Comparable<Event> {
    private static int eventno=0;
    private final double time;  // time that event is scheduled to occur
    private final EventType type;
    private final int data;     // Data for this event
    private final int data2;    // Additional data for this event
    private final Packet p;
    
    // create a new event to occur at time t involving
    public Event(double t, EventType type, int data, int data2, Packet p) {
      this.eventno++;
      this.time  = t;
      this.type  = type;
      this.data  = data;
      this.data2 = data2;
      this.p     = p;
    }
    
    // compare times when two events will occur
    public int compareTo(Event that) {
      if      (this.time < that.time) return -1;
      else if (this.time > that.time) return +1;
      else                            return  0;
    }
  }
  
  public static void main(String [] args) {
    if (args.length != 2) {
        System.out.println("Usage: Sim packets size\n");
        return;
    }
    int packets = Integer.parseInt(args[0]);
    int size    = Integer.parseInt(args[1]);
    System.out.println("Packets: " + packets);
    System.out.println("Size   : " + size);
    Sim2 bgan = new Sim2(packets, size);
    bgan.simulate();
  }
}
