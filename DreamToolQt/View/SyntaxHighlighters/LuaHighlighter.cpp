/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Dieses Programm ist Freie Software: Sie können es unter den Bedingungen
 * der GNU General Public License, wie von der Free Software Foundation,
 * Version 3 der Lizenz oder (nach Ihrer Option) jeder späteren
 * veröffentlichten Version, weiterverbreiten und/oder modifizieren.
 *
 * Dieses Programm wird in der Hoffnung, dass es nützlich sein wird, aber
 * OHNE JEDE GEWÄHRLEISTUNG, bereitgestellt; sogar ohne die implizite
 * Gewährleistung der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK.
 * Siehe die GNU General Public License für weitere Details.
 *
 * Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
 * Programm erhalten haben. Wenn nicht, siehe <http://www.gnu.org/licenses/>.
 */

#include "LuaHighlighter.h"
#include <cstdio>

LuaHighlighter::LuaHighlighter(QObject *parent) :
    QSyntaxHighlighter(parent)
{
    endColor.setRgb(0x00, 0x00, 0xFF);
    commentColor.setRgb(0x08, 0xA0, 0x80);
}

void LuaHighlighter::highlightBlock(const QString& text)
{
    QFont bold;
    bold.setBold(true);

    for(int i = 0; i < text.length(); i++)
    {
        if(text.at(i).isNumber())
        {
            setFormat(i, 1, QColor(0xFF, 0x80, 0x00));
        }

        QString edited = text;
        edited = edited.replace("\t", " ");

        QStringList list = edited.split(" ");
        QString word;

        for(int i = 0; i < list.size(); i++)
        {
            word = list.at(i);
            if(word == "end")
                setFormat(text.indexOf(word), word.length(), endColor);
            else if(word == "function")
                setFormat(text.indexOf(word), word.length(), endColor);
            else if(word == "if" || word == "else" || word == "then" || word == "elseif" || word == "nil" || word == "for" || word == "do")
                setFormat(text.indexOf(word), word.length(), bold);
        }

        int idx;
        // Keywords
        /*int idx = text.indexOf("end");

        if(idx != -1)
            setFormat(idx, 3, endColor);

        idx = text.indexOf("function");

        if(idx != -1)
            setFormat(idx, 8, endColor);


        // Bold
        idx = text.indexOf("if");

        if(idx != -1)
            setFormat(idx, 2, bold);

        idx = text.indexOf("else");

        if(idx != -1)
            setFormat(idx, 4, bold);


        idx = text.indexOf("then");

        if(idx != -1)
            setFormat(idx, 4, bold);

        idx = text.indexOf("for");

        if(idx != -1)
            setFormat(idx, 3, bold);

        idx = text.indexOf("do");

        if(idx != -1)
            setFormat(idx, 2, bold);

        idx = text.indexOf("nil");

        if(idx != -1)
            setFormat(idx, 3, bold);*/

        // Comments
        idx = text.indexOf("--");
        int comment = idx;

        if(idx != -1)
        {
            setFormat(idx, text.length() - idx, commentColor);
        }

        // Quotes
        idx = text.indexOf("\"");

        if(idx != -1 && (idx < comment || comment == -1))
        {
            int idx2 = text.indexOf("\"", idx + 1);

            if(idx2 != -1)
            {
                idx2++;
                setFormat(idx, idx2 - idx, Qt::red);
            }
        }
    }
}
