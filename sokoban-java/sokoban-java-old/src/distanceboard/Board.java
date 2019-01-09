package distanceboard;

import distanceboard.grid.GridCell;
import distanceboard.grid.GridPane;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.LinkedList;

public class Board extends JFrame implements ActionListener, GridCell.CellClickedListener
{
    private GridPane gridPane;
    private JPanel panel;

    private JRadioButton modeWall;
    private JRadioButton modeExec;

    private JRadioButton alg1;
    private JRadioButton alg2;

    private JCheckBox boxTargetMode;
    private JRadioButton placeBox;
    private JRadioButton placeWall;
    private JRadioButton placeTarget;
    private JButton execute;

    private JButton resetButton;

    private int mode = 0; // 0 - calculate for every cell, 1 - calculate paths

    private int selectMode = 0; // 0 - execute, 1 - place wall, 2 - place target, 3 - place box
    private int alg  = 1;

    private int cols = 11;
    private int rows = 11;

    private GridCell boxCell = null;

    public Board()
    {
        super();
        this.setTitle("Algorithm Preview Board");
        this.setVisible(true);
        this.setLayout(new BorderLayout(10, 0));
        this.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        this.setBackground(Color.gray);
        this.setPreferredSize(new Dimension(1200, 1000));

        gridPane = new GridPane(this, cols, rows);
        gridPane.setLocation(0, 0);
        this.add(gridPane, BorderLayout.CENTER);

        panel = new JPanel();
        panel.setLayout(new BoxLayout(panel, BoxLayout.Y_AXIS));

        panel.add(Box.createRigidArea(new Dimension(0, 20)));
        modeWall = new JRadioButton("Place Walls");
        modeWall.setFont(modeWall.getFont().deriveFont(25f));
        modeWall.addActionListener(this);
        panel.add(modeWall);
        modeExec = new JRadioButton("Execute");
        modeExec.setFont(modeExec.getFont().deriveFont(25f));
        modeExec.addActionListener(this);
        panel.add(modeExec);

        panel.add(Box.createRigidArea(new Dimension(0, 50)));
        alg1 = new JRadioButton("Algorithm 1");
        alg1.setFont(alg1.getFont().deriveFont(25f));
        alg1.setSelected(this.alg == 1);
        alg1.addActionListener(this);
        panel.add(alg1);
        alg2 = new JRadioButton("Algorithm 2");
        alg2.setFont(alg2.getFont().deriveFont(25f));
        alg2.setSelected(this.alg == 2);
        alg2.addActionListener(this);
        panel.add(alg2);

        panel.add(Box.createRigidArea(new Dimension(0, 50)));
        boxTargetMode = new JCheckBox("Box and Target mode");
        boxTargetMode.setFont(boxTargetMode.getFont().deriveFont(25f));
        boxTargetMode.setSelected(false);
        boxTargetMode.addActionListener(this);
        panel.add(boxTargetMode);

        placeBox = new JRadioButton("Place Box");
        placeBox.setFont(placeBox.getFont().deriveFont(25f));
        placeBox.addActionListener(this);
        placeBox.setEnabled(false);
        placeBox.setSelected(true);
        panel.add(placeBox);

        placeTarget = new JRadioButton("Place Targets");
        placeTarget.setFont(placeTarget.getFont().deriveFont(25f));
        placeTarget.addActionListener(this);
        placeTarget.setEnabled(false);
        placeTarget.setSelected(false);
        panel.add(placeTarget);

        placeWall = new JRadioButton("Place Walls");
        placeWall.setFont(placeWall.getFont().deriveFont(25f));
        placeWall.addActionListener(this);
        placeWall.setEnabled(false);
        placeWall.setSelected(false);
        panel.add(placeWall);

        execute = new JButton("Execute");
        execute.setFont(execute.getFont().deriveFont(25f));
        execute.addActionListener(this);
        execute.setEnabled(false);
        panel.add(execute);

        panel.add(Box.createVerticalGlue());

        resetButton = new JButton("Reset");
        resetButton.setFont(resetButton.getFont().deriveFont(25f));
        resetButton.addActionListener(this);
        panel.add(resetButton);
        panel.add(Box.createRigidArea(new Dimension(0, 30)));

        this.add(panel, BorderLayout.LINE_END);

        this.pack();

    }

    @Override
    public void actionPerformed(ActionEvent e)
    {
        if(e.getSource().equals(modeWall))
        {
            selectMode = 1;
            modeExec.setSelected(false);
        }
        else if(e.getSource().equals(modeExec))
        {
            selectMode = 0;
            modeWall.setSelected(false);
        }
        else if(e.getSource().equals(alg1))
        {
            alg = 1;
            alg2.setSelected(false);
        }
        else if(e.getSource().equals(alg2))
        {
            alg = 2;
            alg1.setSelected(false);
        }
        else if(e.getSource().equals(resetButton))
        {
            gridPane.resetAll();
        }
        else if(e.getSource().equals(boxTargetMode))
        {
            if(boxTargetMode.isSelected())
            {
                mode = 1;
                selectMode = 3;
                modeWall.setEnabled(false);
                modeExec.setEnabled(false);
                alg1.setEnabled(false);
                alg2.setEnabled(false);

                placeBox.setEnabled(true);
                placeBox.setSelected(true);
                placeWall.setEnabled(true);
                placeWall.setSelected(false);
                placeTarget.setEnabled(true);
                placeTarget.setSelected(false);
                if(boxCell != null) execute.setEnabled(true);
                else execute.setEnabled(false);
            }
            else {
                mode = 0;
                selectMode = 1;
                modeWall.setEnabled(true);
                modeWall.setSelected(true);
                modeExec.setEnabled(true);
                modeExec.setSelected(false);
                alg1.setEnabled(true);
                alg2.setEnabled(true);

                placeBox.setEnabled(false);
                placeWall.setEnabled(false);
                placeTarget.setEnabled(false);
                execute.setEnabled(false);
            }
        }
        else if(e.getSource().equals(placeWall))
        {
            selectMode = 1;
            placeWall.setSelected(true);
            placeTarget.setSelected(false);
            placeBox.setSelected(false);
        }
        else if(e.getSource().equals(placeTarget))
        {
            selectMode = 2;
            placeTarget.setSelected(true);
            placeWall.setSelected(false);
            placeBox.setSelected(false);
        }
        else if(e.getSource().equals(placeBox))
        {
            selectMode = 3;
            placeBox.setSelected(true);
            placeTarget.setSelected(false);
            placeWall.setSelected(false);
        }
        else if(e.getSource().equals(execute))
        {
            LinkedList<GridCell> targets = new LinkedList<>();
            for(int i = 0; i < cols; i++)
                for(int j = 0; j <  rows; j++)
                    if(gridPane.get(i, j).isTarget())
                        targets.addLast(gridPane.get(i, j));

            if(boxCell != null)
                Algorithm.calculatePath2(gridPane, cols, rows, boxCell, targets.toArray(new GridCell[targets.size()]));
        }
    }

    @Override
    public void clicked(GridCell cell)
    {
        if(selectMode == 0)
        {
            if(alg == 1) Algorithm.calculatePath1(gridPane, cols, rows, cell);
            else if(alg == 2) Algorithm.calculatePath2(gridPane, cols, rows, cell, null);
        }
        else if(selectMode == 1)
        {
            if(!cell.isWall()) cell.setWall();
            else cell.reset();
        }
        else if(selectMode == 2)
        {
            if(!cell.isTarget()) cell.setTarget();
            else cell.reset();
        }
        else if(selectMode == 3)
        {
            if(!cell.isBox())
            {
                gridPane.moveBox(cell);
                boxCell = cell;
                execute.setEnabled(true);
            }
            else
            {
                cell.reset();
                boxCell = null;
                execute.setEnabled(false);
            }
        }
    }

    public static void main(String[] args)
    {
        Board board = new Board();
    }
}
