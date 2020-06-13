package simulation;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.WindowConstants;

public class ParticlesView implements Runnable {
    
    private ParticlesModel model;
    private JPanel         screen;
    private String         name;
    
    public ParticlesView(String title, ParticlesModel m) {
        model  = m;
        name   = title;
    }

    @Override
    public void run() {
        createAndShowGUI();        
    }

    private void createAndShowGUI() {
        JFrame frame = new JFrame(name);
        frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
    
        screen = new JPanel(){

            @Override
            public void paintComponent(Graphics g) {
                super.paintComponent(g);
                Graphics2D g2d = (Graphics2D) g;
                g2d.setRenderingHint(
                        RenderingHints.KEY_ANTIALIASING,
                        RenderingHints.VALUE_ANTIALIAS_ON);
                for (Particle p : model.getParticles()) {
                    g2d.setColor(p.colour());
                    g2d.fill(p.represent());
                }
            }

            @Override
            public Dimension getPreferredSize() {
                return model.getSize();
            }
            
        };
        screen.setBackground(Color.WHITE);
        frame.add(screen);
        frame.pack();
        frame.setVisible(true);
    }


    public void update() {
        screen.repaint();
    }
    
}


