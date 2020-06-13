package simulation;

import java.lang.reflect.InvocationTargetException;
import javax.swing.SwingUtilities;
import utils.MinPriorityQueue;

public class ParticleSimulation implements Runnable, ParticleEventHandler{

    private MinPriorityQueue<Event> events;
    private double time;

    private static final long FRAME_INTERVAL_MILLIS = 40;
    
    private final ParticlesModel          model;
    private final ParticlesView           screen;
    
    /**
     * Constructor.
     */
    public ParticleSimulation(String name, ParticlesModel m) {
        model = m;
        time = 0;
        screen = new ParticlesView(name, m);
        events = new MinPriorityQueue<>();
        events.add(new Tick(1));
        for (Event e: model.predictAllCollisions(0)) {
            events.add(e);
        }
    }

    /**
     * Runs the simulation.
     */
    @Override
    public void run() {
        try {
            SwingUtilities.invokeAndWait(screen);
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        while (!events.isEmpty()) {
            Event event = events.remove();
            if (event.isValid()) {
                model.moveParticles(event.time() - this.time);
                event.happen(this);
                this.time = event.time();
            }
        }
    }

    @Override
    public void reactTo(Tick tick) {
        try {
            Thread.sleep(FRAME_INTERVAL_MILLIS);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        screen.update();
        events.add(new Tick(time + 1));
    }

    @Override
    public void reactTo(Collision c) {
        for (Particle p: c.getParticles()) {
            for (Event e: model.predictCollisions(p, time)) {
                events.add(e);
            }
        }
    }
}
