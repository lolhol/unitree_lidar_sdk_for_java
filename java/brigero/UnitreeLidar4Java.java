package brigero;

public class UnitreeLidar4Java {
  static {
    System.loadLibrary("unitreelidar4java");
    //System.out.println(System.getProperty("java.library.path"));
  }

  private long native_ptr;

  public native void init(String pathToLidar);

  public native void setStateImpl(int state);

  public native PointCloud getPointCloudObject();

  public native IMUUnitree getIMUData();

  public native float getPercentDirty();

  public native int getCurMessageInt();

  public native void setLidarLEDModeImpl(int state);

  public void setLidarLEDMode(LEDDisplayMode state) {
      this.setLidarLEDModeImpl(state.value);
  }

  public MessageType getCurMessageEnum() {
    int cur = getCurMessageInt();
      return switch (cur) {
          case 0 -> MessageType.NONE;
          case 1 -> MessageType.IMU;
          case 2 -> MessageType.POINTCLOUD;
          case 3 -> MessageType.RANGE;
          case 4 -> MessageType.AUXILIARY;
          case 5 -> MessageType.VERSION;
          case 6 -> MessageType.TIMESYNC;
          default -> MessageType.NULL;
      };
  }

  public void setState(UnitreeLidar4JavaStates state) {
    setStateImpl(state.value);
  }
}
