package simulation;

import java.awt.Color;

public class SimpleSimulation {

    private static int N = 50;
    
    public static void main(String[] args) {
        int w  = 600;
        int h  = 600;

        Particle[] ps = new Particle[N];
        for (int i = 0; i < N; i++) {
            int x = (int) (Math.random() * (w - Particle.BIG)
                    + Particle.BIG / 2);
            int y = (int) (Math.random() * (w - Particle.BIG)
                    + Particle.BIG / 2);
            double dx = 0, dy = 0;
            while (dx == 0) {
                dx = Math.random() * 11 - 5;
            }
            while (dy == 0) {
                dy = Math.random() * 11 - 5;
            }
            ps[i] = new Particle(x, y, dx, dy, Particle.BIG, 1, Color.BLUE);
        }
        
        ParticlesModel     model = new ParticlesModel(w, h, ps);
        ParticleSimulation sim = 
                new ParticleSimulation("Elastic Collisions in 2D", model);        
        sim.run();
    }

}


