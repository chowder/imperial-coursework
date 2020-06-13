package simulation;

public abstract class AbstractEvent implements Event {

    double eventTime;

    /**
     * Constructor for AbstractEvent.
     */
    public AbstractEvent(double time) {
        this.eventTime = time;
    }

    /**
     * Returns the time (in ticks) at which this event will occur.
     */
    @Override
    public double time() {
        return this.eventTime;
    }

    /**
     * Compares this object with the specified Event.
     */
    @Override
    public int compareTo(Event that) {
        double diff = this.time() - that.time();
        if (diff > 0) {
            return 1;
        }
        else if (diff < 0) {
            return -1;
        }
        else {
            return 0;
        }
    }

}
