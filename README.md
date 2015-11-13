# TP5

Pour texturer le terrain, je me suis servi des shader, j'ai donc créé un VertexShader et un FragmentShader

Le vertex Shader :

Le programme de gestion des vertex est contenu dans le fichier vertex.glsl. Ce programme positionne les points en multipliant la matrice de transformations d'openGL par le vecteur de position.

Le programme de gestion des pixels se trouve dans le fichier fragement.glsl. Ce programme modifie pour chaque pixel en leur modifiant leur couleur. C'est ici que les normales sont utilisées pour la gestion de la lumière et l'affichage des textures.

Les points de la scène sont désormais stockés en VAO, grace aux fonctions  glEnableCLientState(), glVertexPointer() et glNormalPointer(). Les points et les normales sont stockés dans un QVector<QVector3D> et affichés grâce à la fonction glDrawArrays().
