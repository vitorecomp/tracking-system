Para compilar o codigo, simplemente execute
make clean
make

Para executar com as configuracoes padroes execute sem parametros
./release/neural_net

Para passar parametros
./release/neural_net ./resource/config.json

Lembre-se o progama depende da estrutura de pastas
Nas configuracoes tempos os seuginte paramentros, no formato json

{
"test_image_file":"...",
"test_result_file":"...",

"to_test":true | false,
"to_train":true | false,
"training_image_file":"./resource/t10k-images-idx3-ubyte",
"training_result_file":"./resource/t10k-labels-idx1-ubyte",

"num_epocs":900,
"learnRate":0.05,
"momen":0.002,
"neurons":70,
"mul":1,
"sum":0,

"nn_struct":[
	[num_nerons_lay_1, num_nerons_lay_2, num_nerons_lay_3],
 	[segunda rede com uma camada], ...
	 ]
}

exemplo de nn struct
[[10, 20, 10], [10], [12, 12], [20]]