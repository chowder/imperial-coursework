package simulation;

import simulation.ParticlesModel.Direction;

public class Wall{

    private Direction direction;
    private int location;

    public Wall(Direction dir, int loc) {
        direction = dir;
        location = loc;
    }

    public Direction direction() {
        return direction;
    }

    public int location() {
        return location;
    }

    @Override
    public String toString() {
        return direction.toString();
    }

}


