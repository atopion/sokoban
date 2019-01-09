package distanceboard;

import distanceboard.grid.GridCell;
import distanceboard.grid.GridPane;

import java.awt.*;
import java.util.*;
import java.util.stream.Collector;

public class Algorithm
{

    /*
     * Erster, einfacher Algorithmus.
     * Berechnet den Weg vom Punkt Origin zum Punkt Target.
     * Dabei wird nur die Bewegungen nach Norden, Süden, Westen, Osten
     * berücksichtigt. Keine Diagonalbewegung.
     */

    public static void calculatePath1(GridPane gridPane, int cols, int rows, GridCell origin)
    {
        gridPane.resetNormal();
        int x = origin.getPosition().x;
        int y = origin.getPosition().y;

        for(int i = 0; i < cols; i++)
        {
            for(int j = 0; j < rows; j++)
            {
                int dist = Math.abs(i - x) + Math.abs(j - y); // Path calculation
                if(gridPane.get(i, j).isWall()) continue;
                if(dist > 10) continue;
                //this.storage[i1][i2].setColor(Color.getHSBColor(0.4687f + (dist - 1) * 0.02344f, 0.6094f, 0.5078f));
                gridPane.get(i, j).setColor(Color.getHSBColor(0.40f + (dist - 1) * 0.043f, 0.6094f, 0.5078f));
            }
        }
    }


    /**
     * Ein einfacher Djikstra Algorithmus, der entweder die kürzesten Wege von einem Startpunkt (origin) zu mehreren
     * Zielpunkten (targets) berechnet, oder die Distanz zum Startpunkt (origin) an jedem Punkt farblich darstellt (in
     * diesem Fall: targets == null).
     *
     * @param gridPane Das Board, welches die Zellen hält.
     * @param cols Anzahl der Spalten.
     * @param rows Anzahl der Reihen.
     * @param origin GridCell mit dem Startpunkt.
     * @param targets GridCell Array mit den Zielen, null für eine Farbdarstellung der Entfernung aller Punkte.
     */

    public static void calculatePath2(GridPane gridPane, int cols, int rows, GridCell origin, GridCell[] targets)
    {
        gridPane.resetNormal();
        int origin_x = origin.getPosition().x;
        int origin_y = origin.getPosition().y;

        //Initialisierung
        int count = 0;
        int[][] distance = new int[cols][rows];
        Point[][] previous = new Point[cols][rows];
        boolean[][] graph = new boolean[cols][rows];
        for(int i = 0; i < distance.length; i++)
        {
            for(int j = 0; j < distance[i].length; j++)
            {
                if(i == origin_x && j == origin_y) distance[i][j] = 0;
                else {
                    distance[i][j] = Integer.MAX_VALUE-100;
                    if(!gridPane.get(i, j).isWall()) count++;
                }
                previous[i][j] = null;
                graph[i][j] = !gridPane.get(i, j).isWall();
            }
        }

        for(int k = 0; k < count; k++)
        {
            // Zelle mit minimalem Abstand finden
            int min = Integer.MAX_VALUE-100;
            int x = 0, y = 0;
            for(int i = 0; i < cols; i++)
            {
                for(int j = 0; j < rows; j++)
                {
                    if(distance[i][j] < min && graph[i][j])
                    {
                        x = i;
                        y = j;
                        min = distance[i][j];
                    }
                }
            }

            // Zelle entfernen
            graph[x][y] = false;

            // Distanzupdate für nord, süd, west, ost
            // Nord
            Point north = new Point(x, y-1);
            if(north.y >= 0 && graph[north.x][north.y])
            {
                if(distance[x][y] +1 < distance[north.x][north.y])
                {
                    distance[north.x][north.y] = distance[x][y] +1;
                    previous[north.x][north.y] = new Point(x, y);
                }
            }
            Point south = new Point(x, y+1);
            if(south.y < rows && graph[south.x][south.y])
            {
                if(distance[x][y] +1 < distance[south.x][south.y])
                {
                    distance[south.x][south.y] = distance[x][y] +1;
                    previous[south.x][south.y] = new Point(x, y);
                }
            }
            Point west = new Point(x-1, y);
            if(west.x >= 0 && graph[west.x][west.y])
            {
                if(distance[x][y] +1 < distance[west.x][west.y])
                {
                    distance[west.x][west.y] = distance[x][y] +1;
                    previous[west.x][west.y] = new Point(x, y);
                }
            }
            Point east = new Point(x+1, y);
            if(east.x < cols && graph[east.x][east.y])
            {
                if(distance[x][y] +1 < distance[east.x][east.y])
                {
                    distance[east.x][east.y] = distance[x][y] +1;
                    previous[east.x][east.y] = new Point(x, y);
                }
            }
        }

        if(targets == null)
        {
            // Jeden Punkt des Rasters entsprechend seiner Distanz zum Startpunkt färben
            for(int i = 0; i < cols; i++)
            {
                for(int j = 0; j < rows; j++)
                {
                    if(i == origin_x && j == origin_y) continue;
                    if(distance[i][j] > 20) continue;
                    gridPane.get(i, j).setColor(Color.getHSBColor((distance[i][j] / 20.0f) * 0.7422f, 0.6172f, 0.4883f));
                }
            }
        }
        else {
            // Die Wege zu jedem Zielpunkt berechnen.
            for(GridCell cell : targets)
            {
                LinkedList<Point> path = new LinkedList<>();
                int cell_x = cell.getPosition().x;
                int cell_y = cell.getPosition().y;

                while(previous[cell_x][cell_y] != null)
                {
                    int tmp_x = cell_x;
                    cell_x = previous[cell_x][cell_y].x;
                    cell_y = previous[tmp_x][cell_y].y;
                    path.addLast(new Point(cell_x, cell_y));
                }

                // Letztes Element ist der Startknoten (Box).
                path.removeLast();
                cell_x = cell.getPosition().x;
                cell_y = cell.getPosition().y;

                for(Point p : path)
                {
                    if(Math.abs(cell_y - p.y) > 0)
                    {
                        if(gridPane.get(cell_x, cell_y).isPath_vert())
                            gridPane.get(cell_x, cell_y).setPath_cross();
                        gridPane.get(p.x, p.y).setPath_horiz();
                    }
                    else if(Math.abs(cell_x - p.x) > 0)
                    {
                        if(gridPane.get(cell_x, cell_y).isPath_horiz())
                            gridPane.get(cell_x, cell_y).setPath_cross();
                        gridPane.get(p.x, p.y).setPath_vert();
                    }

                    cell_x = p.x;
                    cell_y = p.y;
                }
            }
        }
    }
}
