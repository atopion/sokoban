package distanceboard.grid;

import distanceboard.Algorithm;

import javax.swing.*;
import java.awt.*;
import java.util.Random;

public class GridPane extends JPanel
{
    private Random rnd;
    private GridCell[][] storage;

    private int rows;
    private int cols;

    public GridPane(GridCell.CellClickedListener listener,  int cols, int rows)
    {
        super();

        this.cols = cols;
        this.rows = rows;

        storage = new GridCell[cols][rows];
        this.setLayout(new GridLayout(rows, cols, 2, 2));

        for(int col = 0; col < cols; col++)
        {
            for(int row = 0; row < rows; row++)
            {
                GridCell cell = new GridCell(listener, col, row);
                cell.reset();
                storage[col][row] = cell;
                add(cell);
            }
        }
    }

    public GridCell get(int col, int row)
    {
        if(col < 0 || col >= cols || row < 0 || row >= rows) return null;
        return this.storage[col][row];
    }

    public void resetNormal()
    {
        for(int i = 0; i < cols; i++)
            for(int j = 0; j < rows; j++)
                if(this.storage[i][j].isNormal())
                    this.storage[i][j].reset();
    }

    public void resetAll()
    {
        for(int i = 0; i < cols; i++)
            for(int j = 0; j < rows; j++)
                this.storage[i][j].reset();
    }

    public void moveBox(GridCell cell)
    {
        for(int i = 0; i < cols; i++)
            for(int j = 0; j < rows; j++)
                if(get(i, j).isBox()) get(i, j).reset();

        cell.setBox();
    }

    public GridCell getPlayer()
    {
        for(int i = 0; i < cols; i++)
            for(int j = 0; j < rows; j++)
                if(get(i, j).isPlayer())
                    return get(i, j);
        return null;
    }

    public int getCols() { return cols; }
    public int getRows() { return rows; }
}
