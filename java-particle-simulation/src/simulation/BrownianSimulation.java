package simulation;

import java.awt.Color;

public class BrownianSimulation {
    
    public static void main(String[] args) {
        int w  = 400;
        int h  = 400;
        
        int molecules = 400;
        int mMaxSpeed = 10;
        int mDiam     = 5;
        int mMass     = 2;
        Particle[] ps = new Particle[molecules + 1];
        
        // make the big particle
        ps[0] = new Particle(w/2, h/2, 0, 0, 40, 1000, Color.RED);

        // make the gas molecules
        for (int i = 1; i < ps.length; i++) {
            int x = (int) (Math.random() * w);
            int y = (int) (Math.random() * h);
            double vx = Math.random() * 2 * mMaxSpeed - mMaxSpeed;
            double vy = Math.random() * 2 * mMaxSpeed - mMaxSpeed;
            ps[i] = new Particle(x, y, vx, vy, mDiam, mMass, Color.BLACK);
        }
        
        ParticlesModel   model = new ParticlesModel(w, h, ps);
        ParticleSimulation sim = 
                new ParticleSimulation("Brownian Motion", model);        
        sim.run();
    }

}


