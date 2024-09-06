package brigero;

public class PointCloud {

  public final double stamp;
  public final int id, validPointsNum;
  public final Point[] point;

  public PointCloud(double stamp, int id, int validPointsNum, Point[] point) {
    this.stamp = stamp;
    this.id = id;
    this.validPointsNum = validPointsNum;
    this.point = point;
  }
}
