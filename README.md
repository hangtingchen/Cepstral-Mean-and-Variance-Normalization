# Cepstral_Mean_and_Variance_Normalization

## 使用方法
```Shell
CMVN.exe	<input dir>	<output dir>	<fileList>	<Orders of delta>	<MODE>
```
例如，在文件夹`\CMVN\Debug`下输入如下
```
CMVN.exe	FeatureDim15	new	fileList.txt	3	2
```
表示取3阶差分并进行全局CMVN

## 参数


输入参数必须包含<input dir><output dir><fileList><Orders of delta><MODE>，即一共5个参数。

所有路径的分隔符，请使用`\\`，否则报错

1. <input dir>
输入文件夹

2. <output dir>
输出文件夹

3. <fileList>
文件列表的形式为<输入文件><TAB><输出文件><ENTER>，务必使用此规范，不然会文法打开文件

4. <Orders of delta>
0为不做差分，1为取一次差分，以此类推 

5. <MODE>
0为不进行CMVN，1为进行离线CMVN，2为进行全局CMVN