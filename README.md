# robotchallenge3

## 7/22
> 1. 之前拍的圖片已切好
> <https://drive.google.com/file/d/0B-K9oAvOT9mbb2V3OEcwbklodVk/view?usp=sharing>
> 2. 目前流程: 拍攝題目影像 => 找題目的4個角 => 變形成正方形圖片(640*640) => 切割 => 辨識....
> + ~~根據原圖總體的亮度，必須調整threshold才能得出較滿意的結果，可能需要自動產生threshold的方法 (找角那裡) 或是調圖片亮度~~
> + 目前切割先回到等分切割的方法，稍不準，拍到的圖片之亮度對比度會影響邊緣偵測的效果，且變形後的正方形圖片也沒有原始題目那麼正，根據拍攝情形會有一點點歪(也許也會有一點影響)，這部分可再研究
> + ~~最後切出來的圖片可以指定大小，不知train新model時需要的大小是?目前是40*40~~
> + ~~上述為分開的程式，會先試著整合起來~~

## 7/23
> 1. 模式稍作調整，與辨識的接口改成vector<Mat>(32*32)
> 2. 有放一份編譯好的code在ubuntu@192.168.0.108:Desktop/robotchallenge3
> 3. command line: ./challenge3 resource/snapshot_1.png resource/dark_1.jpg resource/showcorner_1.jpg resource/adjust_1.jpg
> + ~~整合的版本: 還沒加上camera~~

## 7/24
> 1. 加入camera的部分

## 7/27
> 1. 找直線加入 hough transform
> 2. 動態調整threshold

## 8/4
> 1. 濾光改成用"右邊5%的牆壁做標準"
> 2. 找線為 hough transform + 平均斜率
> + bug: hough transform 有時會偵測到非邊緣直線，需要一個方法過濾掉
> + 目前使用640*480，像素調大後須調整一次各項參數