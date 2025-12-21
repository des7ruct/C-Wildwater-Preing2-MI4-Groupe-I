# C-Wildwater-Preing2-MI4-Groupe-I

Pour utiliser l'application il suffit simplement d'extraire les fichiers du répertoire git dans un dossier, de se placer dans celui si et d'exécuter le script shell avec la syntaxe suivante,


pour l'histogramme :

bash wildwater.shell {chemin du fichier CSV de données} histo {max / src / real}


pour les fuites :

bash wildwater.shell {chemin du fichier CSV de données} leaks {id d'une usine}


si le chemin du fichier n'est pas détecté pour quelque raison que ce soit il suffit de mettre le fichier
de données dans le même répertoire que le reste du code, et de remplacer le chemin du fichier de données par son nom

pour une raison inconnue le code ne fonctionne parfois pas à la première tentative (erreur la valeur retour du programme C non égale à 0) mais fonctionne à la deuxième, exécuter la même commande deux fois de suite peut fonctionner.
