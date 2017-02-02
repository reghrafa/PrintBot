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
using Android.Webkit;

namespace PrintBot.Droid.Fragments
{
    public class CodeViewFragment : Fragment
    {

        private CodeEditorViewModel _codeEditorViewModel = ServiceLocator.Current.CodeEditorViewModel;

        private string _htmlBase = @"<!DOCTYPE html>
<html>
<head>
    <meta charset=""utf-8"" />
    <link rel=""stylesheet"" href=""prettify.css"" />
    <script src=""prettify.js""></script>
    <script>
        window.onload = (function() {{ prettyPrint(); }});
    </script>
</head>
<body>
    <pre class=""prettyprint"" style=""padding:0; border:none;"">
    {0}
    </pre>
</body>
</html>";


        public override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            
        }
        
        public override View OnCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
        {
            var view = inflater.Inflate(Resource.Layout.CodeViewFragment, container, false);

            WebView wv = view.FindViewById<WebView>(Resource.Id.CodeView_webView);
            wv.Settings.JavaScriptEnabled = true;
            wv.LoadDataWithBaseURL("file:///android_asset/", string.Format(_htmlBase, _codeEditorViewModel.Code), "text/html", "utf-8", null);
            _codeEditorViewModel.PropertyChanged += (s, e) => {
                if(e.PropertyName == nameof(_codeEditorViewModel.Code))
                {
                    wv.LoadDataWithBaseURL("file:///android_asset/", string.Format(_htmlBase, _codeEditorViewModel.Code), "text/html", "utf-8", null);
                }
            };
            
            return view;
        }
        
    }
}