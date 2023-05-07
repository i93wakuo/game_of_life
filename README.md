# 説明
C言語のcursesを利用したライフゲーム
# 遊び方
q以外のキーを押すと次の世代に移動します。
qキーを押すと終了します
# コンパイル方法
gcc game_of_life.c -lcurses -o game_of_life
# 実行方法
./game_of_life
# 注意
game_map.hはbitmapで書き換えてください
