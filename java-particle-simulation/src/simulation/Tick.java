package simulation;

public class Tick extends AbstractEvent {

    public Tick(double time) {
        super(time);
    }

    @Override
    public boolean isValid() {
        return true;
    }

    @Override
    public void happen(ParticleEventHandler h) {
        h.reactTo(this);
    }
}
