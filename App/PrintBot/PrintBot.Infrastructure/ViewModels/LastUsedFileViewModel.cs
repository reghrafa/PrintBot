using PrintBot.Infrastructure.Mvvm;
using PrintBot.Domain.Models;
using System;
using System.IO;
using Newtonsoft.Json;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using PCLStorage;

namespace PrintBot.Infrastructure.ViewModels
{
    public class LastUsedFileViewModel : ViewModelBase
    {
        public List<FileModel> FileList = new List<FileModel>();
        
        private string _projectsFilename = "projects.json";
        private IFolder _folder;

        public LastUsedFileViewModel()
        {  
        }
        
        public async Task LoadData()
        {
            IFolder rootFolder = FileSystem.Current.LocalStorage;
            _folder = await rootFolder.CreateFolderAsync("ProjectFiles",
                CreationCollisionOption.OpenIfExists);

            IFile file = await _folder.CreateFileAsync(_projectsFilename,
                CreationCollisionOption.ReplaceExisting);
            string content = await file.ReadAllTextAsync();
            FileList = JsonConvert.DeserializeObject<List<FileModel>>(content);
        }
        public string ChangeCreationDate(int position)
        {
            FileList[position].CreationDate = DateTime.Now;
            return FileList[position].Path;
        }

        public async Task AddFile(string fileName)
        {
            var rand = new Random();
            IFile file = await _folder.CreateFileAsync(fileName +".c",
                CreationCollisionOption.ReplaceExisting);
            await file.WriteAllTextAsync("test" + rand.Next(123781));
            FileList.Insert(0, new FileModel() { Title = fileName, CreationDate = DateTime.Now, Path = file.Path });
            await WriteAndRefresh();
        }



        private async Task WriteAndRefresh()
        {
            IFile file = await _folder.CreateFileAsync(_projectsFilename,
                CreationCollisionOption.ReplaceExisting);
            var json = JsonConvert.SerializeObject(FileList);
            await file.WriteAllTextAsync(json);
            OnPropertyChanged(nameof(FileList));
        }
    }
}
