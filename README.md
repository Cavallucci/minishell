<!DOCTYPE html>
<html>

<head>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 40px;
        }

        h1 {
            color: #333;
            border-bottom: 1px solid #ccc;
        }

        h2 {
            color: #555;
        }

        pre {
            background-color: #f5f5f5;
            padding: 10px;
            border-radius: 5px;
        }

        code {
            font-family: Consolas, monospace;
        }
    </style>
</head>

<body>
    <h1>Minishell</h1>

    <p>Bienvenue dans le projet Minishell ! Ce projet consiste à créer un shell minimaliste en langage C, offrant un interpréteur de commandes basique. Ce shell vous permettra d'exécuter des commandes système, de gérer des processus, ainsi que quelques fonctionnalités supplémentaires.</p>

    <h2>Fonctionnalités principales</h2>
    <ul>
        <li>Interprétation des commandes système</li>
        <li>Gestion des processus</li>
        <li>Variables d'environnement</li>
        <li>Redirections</li>
        <li>Pipes</li>
    </ul>

    <h2>Configuration requise</h2>
    <ul>
        <li>Système d'exploitation : Linux, macOS ou Windows (avec l'émulateur WSL)</li>
        <li>Compilateur : GCC (GNU Compiler Collection)</li>
    </ul>

    <h2>Installation</h2>
    <ol>
        <li>Clonez ce dépôt GitHub sur votre machine locale :</li>
    </ol>

    <pre><code>git clone https://github.com/Cavallucci/Minishell.git</code></pre>

    <ol start="2">
        <li>Accédez au répertoire du projet :</li>
    </ol>

    <pre><code>cd Minishell</code></pre>

    <ol start="3">
        <li>Compilez le projet à l'aide du Makefile :</li>
    </ol>

    <pre><code>make</code></pre>

    <ol start="4">
        <li>Exécutez le fichier exécutable généré :</li>
    </ol>

    <pre><code>./minishell</code></pre>

    <h2>Utilisation</h2>
    <p>Une fois que vous avez exécuté le fichier <code>minishell</code>, vous pouvez commencer à utiliser le shell en entrant des commandes. Voici quelques exemples de commandes que vous pouvez essayer :</p>

    <pre><code>$ ls<br>$ cat fichier.txt<br>$ mkdir nouveau_dossier</code></pre>

    <pre><code>$ cd chemin/vers/répertoire</code></pre>

    <pre><code>$ echo $PATH</code></pre>

    <pre><code>$ ls > fichiers.txt<br>$ cat &lt; fichier.txt</code></pre>

    <pre><code>$ ls | grep "motif"</code></pre>

    <p>Pour obtenir plus d'informations sur les fonctionnalités et les commandes prises en charge par Minishell, vous pouvez consulter la documentation intégrée ou les fichiers source du projet.</p>

<h2>Auteurs</h2>
<ul>
    <li>Kralik Marion</li>
    <li>Cavallucci Laura</li>
</ul>

</body>

</html>
