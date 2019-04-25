package view;

import javax.swing.*;
import javax.swing.text.AttributeSet;
import javax.swing.text.BadLocationException;
import javax.swing.text.PlainDocument;
import java.awt.event.*;

public class Name extends JDialog {
    private JPanel contentPane;
    private JButton buttonOK;
    private JTextField textField;

    public Name(ActionListener listener, JFrame parent) {
        super(parent);
        this.setLocationRelativeTo(null);
        setTitle("Save Score");
        setContentPane(contentPane);
        setModal(true);
        getRootPane().setDefaultButton(buttonOK);

        textField.setDocument(new PlainDocument() {
            @Override
            public void insertString(int offs, String str, AttributeSet a) throws BadLocationException {
                if (Character.isSpaceChar(str.charAt(0))) {
                    return;
                }
                if (str.length() + offs > 10) {
                    return;
                }
                super.insertString(offs, str, a);
            }
        });
        buttonOK.setActionCommand("Record");
        buttonOK.addActionListener(listener);

        this.pack();
        setMaximumSize(getSize());
    }

    public String getName() {
        return textField.getText();
    }

    void clear() {
        textField.setText(null);
    }
}
