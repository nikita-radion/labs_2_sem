DIRECTORY="."

help() {
  echo "Использование: test.sh [ОПЦИИ]"
  echo "  --directory DIR   Установить директорию для поиска файлов (по умолчанию: .)"
  echo "  ?                 Показать это сообщение и выйти"
}

while [ "$#" -gt 0 ]; do
  case "$1" in
    --directory)
      DIRECTORY="$2"
      shift 2
      ;;
    ?)
      help
      exit 0
      ;;
    *)
      echo "Неизвестная опция: $1"
      help
      exit 1
      ;;
  esac
done
if find $DIRECTORY -name "*.txt" -print -quit | grep -q '^'; then
    echo "1" 
f=$(find $DIRECTORY -name "*.txt")
for file in $f
do
        enca -L none -x UNICODE "$file"
done
else
    echo "0"  
    echo "В данной директории нет файлов .txt"
fi
