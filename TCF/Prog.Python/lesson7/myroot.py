#!/usr/bin/env python
# coding: utf-8

# In[1]:


# myroot.py

# classe personalizzata per migliorare l'aspetto di istogrammi sovrapposti
from ROOT import TH1F, TCanvas, TLegend


# In[2]:


class MyTemplatePlotter :
    
    def __init__(self) :
        self._templateList = []
        self._data = TH1F()
        self._canvas = TCanvas()
        self._legend = TLegend(0.75,0.7,0.9,0.9)

    def __str__(self) :
        mystr = ''
        for h in self._templateList:
            mystr += f'Template {h.GetName()} : {h.GetTitle()} \n'
        mystr += 'Data : XRF data'
        return mystr

    def AddData(self,data) :
        self._data = data.Clone() 
        self._data.SetName("data")
        self._legend.AddEntry(self._data,"data","lep")

    def AddTemplate(self,templ,symbol,scale=-1.) :
        if (templ.GetNbinsX() != self._data.GetNbinsX() or templ.GetBinCenter(1) != self._data.GetBinCenter(1)) :
            raise Exception("Add data before: templates must have the same binning as data")
        this_templ = templ.Clone()    
        this_templ.Sumw2()    
        if (scale > 0) : 
            this_templ.Scale(scale / this_templ.Integral())
        self._templateList.append(this_templ)
        self._legend.AddEntry(this_templ,symbol,"l")

    def Draw(self,canvas) :
        self._canvas = canvas
        if (len(self._templateList) == 0 or self._data.GetNbinsX() == 0) :
            raise Exception("You need to add templates and data before drawing")
        self._canvas.cd()
        for i in range(len(self._templateList)) :
            self._templateList[i].SetLineColor(i+2)
            if (i > 0):
                self._templateList[i].Add(self._templateList[i-1])
        self._data.SetLineColor(1) ;  # self._data.SetMarkerStyle(21)           
        self._templateList.append(self._data) 
        _sortedList = sorted(self._templateList, key=lambda x : x.GetMaximum(), reverse=True)
        for k in range(len(_sortedList)) :
            opt = ''
            if (k > 0) :
                opt += 'same'
            else:
                _sortedList[k].SetTitle('')
                _sortedList[k].GetXaxis().SetTitle('X-ray energy (keV)')
                _sortedList[k].GetYaxis().SetTitle('Spectrum')
            if (_sortedList[k].GetName() == "data") : 
                opt += 'e' 
            else: 
                opt += 'hist'
            _sortedList[k].Draw(opt) 
        self._legend.Draw("same")
        self._canvas.Update()
        self._canvas.Draw()
            


# In[ ]:




