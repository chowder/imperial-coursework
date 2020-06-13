package simulation;

import java.awt.Color;

public class DiffusionSimulation {

    public static void main(String[] args) {
        int w  = 400;
        int h  = 400;

        int molecules = 200;
        int mDiam     = 5;
        int mMaxSpeed = 5;
        int mMass     = 2;
        int bDiam     = 10;
        int barriers  = h / bDiam - 1;
        int bX        = w / 2;
        int bMass     = 2000000000; 
        
        Particle[] ps = new Particle[molecules + barriers];
        int mXRange = w / 2 - (bDiam / 2 + mDiam);
        int mYRange = h - mDiam;
        
        for (int i = 0, y = bDiam / 2; i < barriers; i++) {
            ps[i] = new Particle(bX, y, 0, 0, bDiam, bMass, Color.BLACK);
            y += bDiam;
            if (i == barriers / 2) { y += bDiam; }
        }

        for (int i = barriers; i < barriers + molecules; i++) {
            int x = (int) (Math.random() * mXRange + mDiam / 2);
            int y = (int) (Math.random() * mYRange + mDiam / 2);
            double vx = Math.random() * 2 * mMaxSpeed - mMaxSpeed;
            double vy = Math.random() * 2 * mMaxSpeed - mMaxSpeed;
            ps[i] = new Particle(x, y, vx, vy, mDiam, mMass, Color.BLUE);
        }

        ParticlesModel model = new ParticlesModel(w, h, ps);
        ParticleSimulation sim = new ParticleSimulation("Diffusion", model);
        sim.run();
    }


}


