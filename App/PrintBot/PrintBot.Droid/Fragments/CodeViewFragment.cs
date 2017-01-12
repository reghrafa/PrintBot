using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Android.App;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Util;
using Android.Views;
using Android.Widget;
using System.Collections.ObjectModel;
using PrintBot.Domain.Models.Blocks;
using PrintBot.Infrastructure.ViewModels;

namespace PrintBot.Droid.Fragments
{
    public class CodeViewFragment : Fragment
    {

        private CodeEditorViewModel _codeEditorViewModel = ServiceLocator.Current.CodeEditorViewModel;


        public override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);

            // Create your fragment here
        }
        
        public override View OnCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
        {
            // Use this to return your custom view for this Fragment
            
            var view = inflater.Inflate(Resource.Layout.CodeViewFragment, container, false);
            _codeEditorViewModel.PropertyChanged += (s, e) => {
                if(e.PropertyName == nameof(_codeEditorViewModel.Code))
                {
                    var justatest = _codeEditorViewModel.Code;
                    view.FindViewById<TextView>(Resource.Id.CodeView_content).Text = _codeEditorViewModel.Code;
                }
            };
            view.FindViewById<TextView>(Resource.Id.CodeView_content).Text = _codeEditorViewModel.Code;
            /*
             *  ToDo 
             *  Mit ServiceLocator CodeEditorViewModel holen und 
             *  notifypropertychanged abonnieren
             *  prüfen ob property Code geändert wurde -> in TextView inhalt von Code anzeigen
             *  
             */
            return view;
        }
        
    }
}