#include "filemanager.h"

FileManager::FileManager(QString filePath)
{
    file.setFileName(filePath);

    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Impossible d'ouvrir le fichier !";
        return;
    }

    QTextStream flux(&file);
    while(!flux.atEnd())
    {
        QString currentLine = flux.readLine();
        if (currentLine.contains("element vertex"))
        {
            QStringList lineElements = currentLine.split(" ");
            nbVertex = lineElements.at(2).toInt();

            points = new Point[nbVertex]();
        }

        if(currentLine.contains("element face"))
        {
            QStringList lineElements = currentLine.split(" ");
            nbFaces = lineElements.at(2).toInt();

            faces = new int*[nbFaces]();
            nbPointPerFace = new int[nbFaces]();
        }

        if(currentLine.contains("end_header"))
        {
            for(int i = 0; i < nbVertex; i++)
            {
                currentLine = flux.readLine();
                QStringList point = currentLine.split(" ");
                points[i].x = point.at(0).toFloat();
                points[i].y = point.at(1).toFloat();
                points[i].z = point.at(2).toFloat();
            }

            for(int i = 0; i < nbFaces; i++)
            {
                currentLine = flux.readLine();
                QStringList face = currentLine.split(" ");
                int nbFace = face.at(0).toInt();
                nbPointPerFace[i] = nbFace;

                faces[i] = new int[nbFace];
                for (int j = 1; j < nbFace; j++)
                {
                    faces[i][j] = face.at(j).toInt();
                }
            }
        }
    }
    file.close();
}

Point FileManager::getPoint(int index) { return points[index]; }

int FileManager::getNbVertex() { return nbVertex; }
int FileManager::getNbFaces() { return nbFaces; }

int* FileManager::getNbPointPerFace() { return nbPointPerFace; }
int** FileManager::getFaces() { return faces; }

