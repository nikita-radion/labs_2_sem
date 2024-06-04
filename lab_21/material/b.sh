f= find . -name \*.txt



for file in $f

do



  enca -L none -x UNICODE $file



done
