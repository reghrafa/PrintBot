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
        public StorageService()
        {

        }
        public async Task WriteFileAsync(string filename, string content)
        {
            IFolder rootFolder = FileSystem.Current.LocalStorage;
            IFolder folder = await rootFolder.CreateFolderAsync("projects",
                CreationCollisionOption.OpenIfExists);
            IFile file = await folder.CreateFileAsync(filename,
                CreationCollisionOption.ReplaceExisting);
            await file.WriteAllTextAsync(content);
        }
        public async Task<string> ReadFileAsync(string filename)
        {
            IFolder rootFolder = FileSystem.Current.LocalStorage;
            IFolder folder = await rootFolder.CreateFolderAsync("projects",
                CreationCollisionOption.OpenIfExists);
            IFile file = await folder.GetFileAsync(filename);
            return await file.ReadAllTextAsync();
        }
    }
}
