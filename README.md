# TP4

Sauvegarde

Pour sauvegarder les maps en cours, il y a dans les classes Camera et TriangleWindow une méthode toSave() retournant une QString.
La méthode toSave() de Camera récupère les informations (en rotation automatique ou pas, l'état d'affichage, la rotation sur l'axe des X, la rotation sur l'axe des Y, et le scale) pour les écrire dans une String.
La méthode toSave() de TriangleWindow fait appel à la méthode toSave() de son attribut Camera et ajoute à la String retournée le nombre de jours et le nombre de mois.
Les informations sont séparées par des ";".
La class FileManager a comme attribut un QVector de TriangleWindow, une méthode pour ajouter des TriangleWindow (appelée dans le main() à chaque création de TriangleWindow) et une méthode save() qui fait appel aux méthodes toSave() des TriangleWindow contenu dans le QVector.
La méthode save() écrit dans un fichier les différentes String récupéré des TriangleWindow en les séparant par un "|".
FileManager est un singleton, lorsque dans une TriangleWindow la touche F6 est pressé, la méthode save() du FileManager est appelée et toutes les informations sont écrite dans le fichier.

Chargement

La class FileManager possède également une méthode load() (appelée lorsque la touche F5 est pressée) qui récupère le contenu du fichier de sauvegarde et fait appel aux fonctions load() des TriangleWindow en passant en paramètre les informations necessaire. Ces informations sont récupérées en splitant la String contenue dans le fichier par "|" et ";". La fonction load() de TriangleWindow modifie les attributs avec les "nouvelles" données.

Chargement d'un modele 3D

Pour lire un fichier .ply, il y a la class PlyLoader qui contient les coordonnées du modèle ainsi qu'un QVectore (vertices) de la structure point et un QVectore de QVectore d'entier(faces). PlyLoader possède deux méthodes : load() et draw().La méthode load() charge les informations contenu dans le fichier dans vertices et faces. Dans un premier temps, la quantité de points est récupéré (après "element vertex"). À la fin du header se trouve les coordonnées des points ainsi que les normales pour chaque point. Viens ensuite les indices des points formant les faces. Les points ainsi que les normales sont stockés dans "vertices" et les faces dans "faces". La méthode draw() ajoute à la matrice les points et les faces du modèle en utilisant "glPushMatrice()". GL_QUADS et GL_TRIANGLES sont utilisé pour dessiner les faces du modèle (en fonction du nombre de points nécessaire pour former la face). Les normales de chaque point sont indiquées avec glNormal3f (les normales sont contenues dans "vertices"). L'affichage ne fonctionne pas (j'appelle la méthode draw() dans la méthode render() de TriangleWindow, mais rien ne s'affiche).
