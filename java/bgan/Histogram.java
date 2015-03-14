public class Histogram {
  int count;
  int bins;
  double [] values;
  
  Histogram(int N) {
    values = new double [N];
  }
  
  double [] histogram(int bins) {
    double [] stat = new double [size()];
    for (int i = 0; i < size(); i++) {
      stat[i] = values[i];
    }
    double [] res = new double [bins];
    double min = StdStats.min(stat);
    double max = StdStats.max(stat);
    
    for (int i = 0; i < size(); i++) {
      double b = (stat[i]-min)/(max-min)*(bins-1);
      res[(int)b]++;
    }
    
    StdDraw.setXscale(min, max);
    StdDraw.setYscale(0, StdStats.max(res));
    StdDraw.line(min, 0, min, StdStats.max(res)); 
    StdDraw.line(min, 0, max, 0);
    for (int i = 0; i < bins; i++) {
      double x = (max-min)*i/(bins-1) + min;
      double y = res[i];
      StdDraw.filledRectangle(x, y/2, .25, y/2);
      
    }
    StdDraw.text(min,-60, "" + min);
    return res;
  }
  
  public int size() { return count; }
  
  public void add(double val) { values[count++] = val; }
  
  public void stats() {
    double [] stat = new double [size()];
    for (int i = 0; i < size(); i++) {
      stat[i] = values[i];
    }
    System.out.println("Min: " + StdStats.min(stat));
    System.out.println("Avg: " + StdStats.mean(stat));
    System.out.println("Max: " + StdStats.max(stat));
    System.out.println("Var: " + StdStats.var(stat));
  }
  
  public static void main(String [] args) {
    Histogram H = new Histogram(152);
    for (int i = 0; i < 100; i++)
      H.add(241.0 + 0.5*i);
    for (int i = 0; i < 50; i++)
      H.add(251.0 + 0.5*i);
    H.stats();
    H.histogram(20);
  }
}