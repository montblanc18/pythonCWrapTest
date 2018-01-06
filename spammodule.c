//本家ドキュメント ->  https://docs.python.jp/3/extending/extending.html
//コンパイル -> https://docs.python.jp/3/extending/building.html#building

#include <Python.h>
/* 
   "Python.h"は<stdio.h>, <string.h>, <error.h>, <stdlib.h>をインクルードしている。
   もしもstdlib.hファイルがなければ、Python.hがmalloc(), free(), realloc()を直接定義する。
 */

// self引数は、モジュールレベルの関数であればモジュールが、メソッドにはオブジェクトインスタンスが渡される。
// agrs引数は、引数の入ったPythonタプルオブジェクトへのポインタになる。
static PyObject *
spam_system(PyObject *self, PyObject *args){
  const char *command;
  int sts;

  /*
	PyArg_ParseTuple()は引数の型をチェックし、Cの値に変換する。
	すべての引数が正しい方を持っていないと、Falseを返す。 --> 呼び出し側でNULLを返すようにする。
	Pythonインタプリタ全体の取り決めとして、
	関数が処理に失敗した場合はエラーを示す値（通常はNULLポインタ）を返さなければならない
  */
  if(!PyArg_ParseTuple(args, "s", &command)){
	return NULL;
  }  
  sts = system(command);

  /*
	PyLong_FromLong(long v):
	引数vから新たなPyLongObjectを生成して返す。失敗した時はNULLを返す。
	この例だと、Py_BuildValue()として整数オブジェクトを返す。
	もしもvoidを返す関数が期待されている場合、
	return Py_None;
	のようにする。
   */
  return PyLong_FromLong(sts);
}


// モジュールのメソッドテーブルと初期化関数を用意する
/*
  spam_system()をPythonプログラムから呼ぶために、関数名とアドレスをmethod tableに列挙する。
 */

static PyMethodDef SpamMethods[] = {
  {"system", spam_system, METH_VARARGS,   /* METH_VARARGSはC関数が使う呼び出し規約をインタプリタに教えるためのフラグ */
   "Execute a shell command."},
  {NULL, NULL, 0, NULL} /* Sentinel */
};

/*
{
  method tableはモジュール定義の構造体から参照されている必要
*/
static struct PyModuleDef spammodule = {
  PyModuleDef_HEAD_INIT,
  "spam", /* name of module */
  NULL, /* module documentation, may be NULL */
  -1, /* size of per-interprenter state of the module,
	   or -1 if the module keep state in global variables */
  SpamMethods
};

/*
  この構造体を、モジュール初期化関数内でインタプリタに渡さねばならない。
  モジュール名を"name"としたとき、初期化関数について
  ・名前はPyInit_name()でなければならない。
  ・モジュールファイル内で定義されているものの内、唯一の非static要素でなければならない。
*/
PyMODINIT_FUNC
PyInit_spam(void){
  return PyModule_Create(&spammodule);
}

/* 
   PyMODINIT_FUNCは戻り値をPyObject+になるよう宣言
   spamモジュールが初めてimportされると、このPyInit_spam()が呼ばれる。
   すると、
   PyInit_spamが呼ばれる
   -> PyModule_Createが呼ばれる
   -> PyMethodDefにもとづいて作られたモジュールに組み込み関数オブジェクトが挿入され、そのポインタが返る。
*/


/*
  Pythonへ埋め込むときには、PyImport_AppendInittab()を使って、初期化テーブルにモジュールを追加する。
  その後に、オプションでモジュールをimportする。
*/
int main(int argc, char *argv[]){
  // wchar_t: ワイド文字(=通常よりも1文字あたりのバイト数が多い)から構成されるワイド文字列
  wchar_t *program = Py_DecodeLocale(argv[0], NULL);
  if(program==NULL){
	fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
	exit(1);
  }

  /* Add a built-in module, before Py_Initialize */
  PyImport_AppendInittab("spam", PyInit_spam);

  /* Pass argv[0] to the Python interpreter */
  Py_SetProgramName(program);

  /* Initialize the Python interpreter. Required. */
  Py_Initialize();

  /* Optionally import the module; alternatively,
	 import can be deferred until the embedded script
	 imports it.*/
  PyImport_ImportModule("spam");

  PyMem_RawFree(program);
  return 0;
  
}
