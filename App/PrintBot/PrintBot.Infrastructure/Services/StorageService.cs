using PCLStorage;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PrintBot.Infrastructure.Services
{
    public class StorageService
    {
        private readonly string _mainFolder = "projects";
        public StorageService()
        {

        }
        public async Task WriteFileAsync(string filename, string content)
        {
            IFolder rootFolder = FileSystem.Current.LocalStorage;
            IFolder folder = await rootFolder.CreateFolderAsync(_mainFolder,
                CreationCollisionOption.OpenIfExists);
            IFile file = await folder.CreateFileAsync(filename,
                CreationCollisionOption.ReplaceExisting);
            await file.WriteAllTextAsync(content);
        }
        public async Task<string> ReadFileAsync(string filename)
        {
            IFolder rootFolder = FileSystem.Current.LocalStorage;
            IFolder folder = await rootFolder.CreateFolderAsync(_mainFolder,
                CreationCollisionOption.OpenIfExists);
            IFile file = await folder.GetFileAsync(filename);
            return await file.ReadAllTextAsync();
        }
        public async Task<bool> FileExistsAsync(string filename)
        {
            IFolder rootFolder = FileSystem.Current.LocalStorage;
            IFolder folder = await rootFolder.CreateFolderAsync(_mainFolder,
                CreationCollisionOption.OpenIfExists);
            var result = await folder.CheckExistsAsync(filename);
            return result == ExistenceCheckResult.FileExists ? true : false;
        }
    }
}
