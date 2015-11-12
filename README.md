# TP5

Pour texturer le terrain, je me suis servit des shader, j'ai donc créé un VertexShader et un ParticuleShader

Le vertex Shader :

Le programme de gestion des vertex est contenu dans le fichier vertex.glsl. Ce programme applique les normales et les coordonées au points.

Le programme de gestion des particules se trouve dans le fichier fragement.glsl.


Pour pouvoir utiliser les shader, j'ai du créer des QVector3D pour stocker les coordonnées des points (lu dans les fichiers map) et un QVector3D de QVector3D pour stocker les normales.

Les shader sont appliquer à chaque tick
