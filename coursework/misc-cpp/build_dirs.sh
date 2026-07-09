#! /bin/sh
CLASS='3680'
echo "Creating the directory '$CLASS' in your home directory..."
sleep 1

# create the base directory that has rwx permission for faculty (group)
# and user, but no read access for others.
mkhwdir ~/$CLASS

# Put a hook into .bash_logout to run a script that will set all files you
# put into ~/$CLASS to be write enabled so faculty can compile and test
# your code.
# 
# delete the line if already there, then add it.
sed /set_permissions.sh/d ~/.bash_logout
echo  /home/stu/derrick/submissions/$CLASS/set_permissions.sh  >> ~/.bash_logout

# now create the file structures, so they are the same for every student.
mkdir ~/$CLASS/wk1
mkdir ~/$CLASS/wk2
mkdir ~/$CLASS/wk3
mkdir ~/$CLASS/wk4
mkdir ~/$CLASS/wk5
mkdir ~/$CLASS/wk6
mkdir ~/$CLASS/wk7
mkdir ~/$CLASS/wk8
mkdir ~/$CLASS/wk9
mkdir ~/$CLASS/wk10
mkdir ~/$CLASS/wk11
mkdir ~/$CLASS/wk12
mkdir ~/$CLASS/wk13
mkdir ~/$CLASS/wk14
mkdir ~/$CLASS/wk15
mkdir ~/$CLASS/project1
mkdir ~/$CLASS/project2
mkdir ~/$CLASS/project3
echo "Your $CLASS directory setup is complete."
sleep 1

