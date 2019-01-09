package distanceboard;

import distanceboard.grid.GridCell;
import distanceboard.grid.GridPane;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.Scanner;

public class Viewer extends JFrame implements ActionListener, GridCell.CellClickedListener
{
    private GridPane gridPane;
    private JPanel panel;

    private JLabel codeFieldLabel;
    private JTextField codeField;
    private JButton run;
    private JButton load;
    private JButton reset;

    private int cols = 10, rows = 10;
    private int width = -1;
    private LinkedList<Move> targets = new LinkedList<>();

    public Viewer(String levelPath, String solutionCode)
    {
        super();
        this.setTitle("Solution Viewer");
        this.setVisible(true);
        this.setLayout(new BorderLayout(10, 0));
        this.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        this.setBackground(Color.gray);
        this.setPreferredSize(new Dimension(1000, 1200));

        gridPane = new GridPane(this, cols, rows);
        gridPane.setLocation(0, 0);
        this.add(gridPane, BorderLayout.CENTER);

        panel = new JPanel();
        panel.setLayout(new BoxLayout(panel, BoxLayout.Y_AXIS));

        panel.add(Box.createRigidArea(new Dimension(0, 25)));
        codeFieldLabel = new JLabel("Submit the solution code:");
        codeFieldLabel.setFont(codeFieldLabel.getFont().deriveFont(25f));
        panel.add(codeFieldLabel);
        panel.add(Box.createRigidArea(new Dimension(0, 10)));

        codeField = new JTextField("");
        codeField.setFont(codeField.getFont().deriveFont(25f));
        panel.add(codeField);
        panel.add(Box.createRigidArea(new Dimension(0, 10)));

        run = new JButton("Run");
        run.setFont(run.getFont().deriveFont(25f));
        run.addActionListener(this);

        load = new JButton("Load");
        load.setFont(load.getFont().deriveFont(25f));
        load.addActionListener(this);

        reset = new JButton("Reset");
        reset.setFont(reset.getFont().deriveFont(25f));
        reset.addActionListener(this);

        JPanel container1 = new JPanel();
        container1.setLayout(new BoxLayout(container1, BoxLayout.LINE_AXIS));
        container1.add(run);
        container1.add(Box.createHorizontalGlue());
        container1.add(load);
        container1.add(Box.createHorizontalGlue());
        container1.add(reset);
        panel.add(container1);

        panel.add(Box.createRigidArea(new Dimension(0 ,25)));


        JPanel container2= new JPanel();
        container2.setLayout(new BoxLayout(container2, BoxLayout.LINE_AXIS));
        container2.add(Box.createRigidArea(new Dimension(30, 0)));
        container2.add(panel);
        container2.add(Box.createRigidArea(new Dimension(30, 0)));
        this.add(container2, BorderLayout.SOUTH);

        this.pack();

        this.gridPane.get(0, 0).setBox();

        if(solutionCode != null)
            this.codeField.setText(solutionCode);

        if(levelPath != null)
        {
            File f = new File(levelPath);
            this.openFile(f);
        }
    }

    public void clicked(GridCell cell)
    {

    }

    public void actionPerformed(ActionEvent event)
    {
        if(event.getSource().equals(run))
        {
            String solution = this.codeField.getText();
            //this.run(solution);
            new Thread(() -> {
                this.run(solution);
            }).start();
        }
        else if(event.getSource().equals(load))
        {
            final FileDialog fd = new FileDialog(this, "Open", FileDialog.LOAD);
            fd.setFile("*.txt");
            fd.setVisible(true);
            String fileName = fd.getFile();
            String directory = fd.getDirectory();
            if(fileName != null)
            {
                File file = new File(directory, fileName);
                this.openFile(file);
            }
        }
        else if(event.getSource().equals(reset))
        {
            gridPane.resetAll();
        }
    }

    private void openFile(File file)
    {
        try {
            Scanner scan = new Scanner(file);
            StringBuilder txt = new StringBuilder();
            while(scan.hasNextLine()) txt.append(scan.nextLine()).append("\n");
            this.load(txt.toString());
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }

    private void load(String level)
    {
        gridPane.resetAll();
        int x = 0, y = 0;
        for(int i = 0; i < level.length(); i++)
        {
            switch (level.charAt(i))
            {
                case '@': // Player
                    gridPane.get(y, x).resizeIcons();
                    gridPane.get(y, x).setPlayer();
                    break;
                case '$': // Box
                    gridPane.get(y, x).resizeIcons();
                    gridPane.get(y, x).setBox();
                    break;
                case '.': // Target
                    targets.add(new Move(x, y));
                    gridPane.get(y, x).resizeIcons();
                    gridPane.get(y, x).setTarget();
                    break;
                case '#': // Wall
                    gridPane.get(y, x).resizeIcons();
                    gridPane.get(y, x).setWall();
                    break;
                case ' ': // Empty
                    gridPane.get(y, x).resizeIcons();
                    gridPane.get(y, x).reset();
                    break;
                case '\n':// New Line
                    if(width == -1)
                        width = x;
                    x = -1; y++;
                    break;
            }
            x++;
        }
    }

    public void run(String solution)
    {
        if(solution.equals("")) return;
        LinkedList<Move> moves = new LinkedList<>();
        String[] parts = solution.split(",");
        for(int i = 0; i < parts.length; i++)
            parts[i] = parts[i].replace("[", "")
                    .replace("]", "").replace(" ", "");
        for(int i = 0; i < parts.length; i += 2)
        {
            Move m = new Move(Integer.parseInt(parts[i]), Integer.parseInt(parts[i+1]));
            moves.add(m);
        }

        if(this.width == -1)
            return;

        for(Move move : moves)
        {
            int x_f = move.from % width;
            int y_f = move.from / width;
            int x_t = move.to % width;
            int y_t = move.to / width;

            GridCell cell = gridPane.getPlayer();
            if(cell != null)
            {
                if(targets.contains(new Move(x_t, y_t))) cell.setTarget();
                else cell.reset();
            }

            gridPane.get(y_f, x_f).resizeIcons();
            gridPane.get(y_f, x_f).setPlayer();

            gridPane.get(y_t, x_t).resizeIcons();
            gridPane.get(y_t, x_t).setBox();

            this.repaint();
            this.revalidate();
            try { Thread.sleep(1500); } catch (Exception e) { e.printStackTrace(); }
        }
    }

    private static class Move
    {
        int from = 0;
        int to = 0;
        Move(int f, int t) { from = f; to = t; }
        public String toString() { return "[" + from + ", " + to + "]"; }
    }

    public static void main(String[] args)
    {
        String[] ar = Arrays.copyOf(args, 2);
        new Viewer(ar[0], ar[1]);
    }
}
