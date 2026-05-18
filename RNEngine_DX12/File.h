#pragma once
#include "stdafx.h"
namespace RNEngine {
	class File
	{
	public:
       /// <summary>
		/// 指定ディレクトリ内のファイル一覧を取得する。
		/// </summary>
		/// <param name="path">検索対象のディレクトリパス（ワイド文字列）</param>
		/// <returns>ディレクトリエントリの配列を返します。</returns>
		static vector<filesystem::directory_entry> GetDirectoryFile(const wstring& path);
		/// <summary>
		/// 指定したファイルが存在するか確認する。
		/// </summary>
		/// <param name="path">ファイルパス（ワイド文字列）</param>
		/// <returns>存在する場合は true を返します。</returns>
		static bool IsExistFile(const wstring& path);
		/// <summary>
		/// ワイド文字列のファイル名部分を抽出する。
		/// </summary>
		/// <param name="filePath">ファイルパス（ワイド文字列）</param>
		/// <returns>ファイル名（拡張子含む）をワイド文字列で返します。</returns>
		static wstring SplitFilename(const wstring& filePath);
		/// <summary>
		/// マルチバイト文字列のファイル名部分を抽出する。
		/// </summary>
		/// <param name="filePath">ファイルパス（マルチバイト文字列）</param>
		/// <returns>ファイル名（拡張子含む）を文字列で返します。</returns>
		static string SplitFilename(const string& filePath);
		/// <summary>
		/// ワイド文字列のディレクトリパス部分を抽出する。
		/// </summary>
		/// <param name="filePath">ファイルパス（ワイド文字列）</param>
		/// <returns>ディレクトリパスをワイド文字列で返します。</returns>
		static wstring SplitFilePath(const wstring& filePath);
		/// <summary>
		/// マルチバイト文字列のディレクトリパス部分を抽出する。
		/// </summary>
		/// <param name="filePath">ファイルパス（マルチバイト文字列）</param>
		/// <returns>ディレクトリパスを文字列で返します。</returns>
		static string SplitFilePath(const string& filePath);
		/// <summary>
		/// 実行ファイルのディレクトリパスを取得する。
		/// </summary>
		/// <returns>実行ファイルが存在するディレクトリのパスを返します。</returns>
		static filesystem::path GetExeDirectory();

		//-------バイナリ-------
		template<class T>
		static void ReadBinary(ifstream& ifs,T* object,uint32_t size = 1,bool useClassSize = true) {
			streamsize readSize = size;
			if (useClassSize) readSize *= sizeof(T);
			ifs.read(reinterpret_cast<char*>(object), readSize);
		}
		template<class T>
		static void SaveBinary(ofstream& ofs, T* object, uint32_t size = 1, bool useClassSize = true) {
			streamsize readSize = size;
			if (useClassSize) readSize *= sizeof(T);
			ofs.write(reinterpret_cast<const char*>(object), readSize);
		}
	};
}
