##
## Auto Generated makefile, please do not edit
##
ProjectName:=Astronom

## Debug
ConfigurationName      :=Debug
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
WorkspacePath          := "/home/willow/prj/amax-hg/Astronomer"
ProjectPath            := "/home/willow/prj/amax-hg/Astronomer"
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Andrei Ivushkin
Date                   :=01.02.2010
CodeLitePath           :="/home/willow/.codelite"
LinkerName             :=g++
ArchiveTool            :=ar rcus
SharedObjectLinkerName :=g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=
DebugSwitch            :=-gstab
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
CompilerName           :=g++
C_CompilerName         :=g++
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=
CmpOptions             :=`fox-config --cflags` -g $(Preprocessors)
LinkOptions            := `fox-config --libs` 
IncludePath            :=  "$(IncludeSwitch)." "$(IncludeSwitch)3d_party/sqlite-3.6.22/include" "$(IncludeSwitch)." 
RcIncludePath          :=
Libs                   :=$(LibrarySwitch)sqlite3 
LibPath                := "$(LibraryPathSwitch)." "$(LibraryPathSwitch)3d_party/sqlite-3.6.22/lib" 


Objects=Astronom/$(IntermediateDirectory)/DraggableView$(ObjectSuffix) Astronom/$(IntermediateDirectory)/GlyphManager$(ObjectSuffix) Astronom/$(IntermediateDirectory)/main$(ObjectSuffix) Astronom/$(IntermediateDirectory)/WheelView$(ObjectSuffix) Astronom/$(IntermediateDirectory)/Astronom$(ObjectSuffix) Astronom/$(IntermediateDirectory)/MainForm$(ObjectSuffix) Astronom/$(IntermediateDirectory)/RectangleView$(ObjectSuffix) 

##
## Main Build Targets 
##
all: $(OutputFile)

$(OutputFile): makeDirStep $(Objects)
	@mkdir -p $(@D)
	$(LinkerName) $(OutputSwitch)$(OutputFile) $(Objects) $(LibPath) $(Libs) $(LinkOptions)

makeDirStep:
	@test -d ./Debug || mkdir -p ./Debug

PreBuild:


##
## Objects
##
Astronom/$(IntermediateDirectory)/DraggableView$(ObjectSuffix): Astronom/DraggableView.cpp Astronom/$(IntermediateDirectory)/DraggableView$(DependSuffix)
	@test -d Astronom/Debug || mkdir -p Astronom/Debug
	$(CompilerName) $(SourceSwitch) "/home/willow/prj/amax-hg/Astronomer/Astronom/DraggableView.cpp" $(CmpOptions) $(ObjectSwitch)Astronom/$(IntermediateDirectory)/DraggableView$(ObjectSuffix) $(IncludePath)
Astronom/$(IntermediateDirectory)/DraggableView$(DependSuffix): Astronom/DraggableView.cpp
	@test -d Astronom/Debug || mkdir -p Astronom/Debug
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MTAstronom/$(IntermediateDirectory)/DraggableView$(ObjectSuffix) -MFAstronom/$(IntermediateDirectory)/DraggableView$(DependSuffix) -MM "/home/willow/prj/amax-hg/Astronomer/Astronom/DraggableView.cpp"

Astronom/$(IntermediateDirectory)/GlyphManager$(ObjectSuffix): Astronom/GlyphManager.cpp Astronom/$(IntermediateDirectory)/GlyphManager$(DependSuffix)
	@test -d Astronom/Debug || mkdir -p Astronom/Debug
	$(CompilerName) $(SourceSwitch) "/home/willow/prj/amax-hg/Astronomer/Astronom/GlyphManager.cpp" $(CmpOptions) $(ObjectSwitch)Astronom/$(IntermediateDirectory)/GlyphManager$(ObjectSuffix) $(IncludePath)
Astronom/$(IntermediateDirectory)/GlyphManager$(DependSuffix): Astronom/GlyphManager.cpp
	@test -d Astronom/Debug || mkdir -p Astronom/Debug
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MTAstronom/$(IntermediateDirectory)/GlyphManager$(ObjectSuffix) -MFAstronom/$(IntermediateDirectory)/GlyphManager$(DependSuffix) -MM "/home/willow/prj/amax-hg/Astronomer/Astronom/GlyphManager.cpp"

Astronom/$(IntermediateDirectory)/main$(ObjectSuffix): Astronom/main.cpp Astronom/$(IntermediateDirectory)/main$(DependSuffix)
	@test -d Astronom/Debug || mkdir -p Astronom/Debug
	$(CompilerName) $(SourceSwitch) "/home/willow/prj/amax-hg/Astronomer/Astronom/main.cpp" $(CmpOptions) $(ObjectSwitch)Astronom/$(IntermediateDirectory)/main$(ObjectSuffix) $(IncludePath)
Astronom/$(IntermediateDirectory)/main$(DependSuffix): Astronom/main.cpp
	@test -d Astronom/Debug || mkdir -p Astronom/Debug
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MTAstronom/$(IntermediateDirectory)/main$(ObjectSuffix) -MFAstronom/$(IntermediateDirectory)/main$(DependSuffix) -MM "/home/willow/prj/amax-hg/Astronomer/Astronom/main.cpp"

Astronom/$(IntermediateDirectory)/WheelView$(ObjectSuffix): Astronom/WheelView.cpp Astronom/$(IntermediateDirectory)/WheelView$(DependSuffix)
	@test -d Astronom/Debug || mkdir -p Astronom/Debug
	$(CompilerName) $(SourceSwitch) "/home/willow/prj/amax-hg/Astronomer/Astronom/WheelView.cpp" $(CmpOptions) $(ObjectSwitch)Astronom/$(IntermediateDirectory)/WheelView$(ObjectSuffix) $(IncludePath)
Astronom/$(IntermediateDirectory)/WheelView$(DependSuffix): Astronom/WheelView.cpp
	@test -d Astronom/Debug || mkdir -p Astronom/Debug
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MTAstronom/$(IntermediateDirectory)/WheelView$(ObjectSuffix) -MFAstronom/$(IntermediateDirectory)/WheelView$(DependSuffix) -MM "/home/willow/prj/amax-hg/Astronomer/Astronom/WheelView.cpp"

Astronom/$(IntermediateDirectory)/Astronom$(ObjectSuffix): Astronom/Astronom.cpp Astronom/$(IntermediateDirectory)/Astronom$(DependSuffix)
	@test -d Astronom/Debug || mkdir -p Astronom/Debug
	$(CompilerName) $(SourceSwitch) "/home/willow/prj/amax-hg/Astronomer/Astronom/Astronom.cpp" $(CmpOptions) $(ObjectSwitch)Astronom/$(IntermediateDirectory)/Astronom$(ObjectSuffix) $(IncludePath)
Astronom/$(IntermediateDirectory)/Astronom$(DependSuffix): Astronom/Astronom.cpp
	@test -d Astronom/Debug || mkdir -p Astronom/Debug
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MTAstronom/$(IntermediateDirectory)/Astronom$(ObjectSuffix) -MFAstronom/$(IntermediateDirectory)/Astronom$(DependSuffix) -MM "/home/willow/prj/amax-hg/Astronomer/Astronom/Astronom.cpp"

Astronom/$(IntermediateDirectory)/MainForm$(ObjectSuffix): Astronom/MainForm.cpp Astronom/$(IntermediateDirectory)/MainForm$(DependSuffix)
	@test -d Astronom/Debug || mkdir -p Astronom/Debug
	$(CompilerName) $(SourceSwitch) "/home/willow/prj/amax-hg/Astronomer/Astronom/MainForm.cpp" $(CmpOptions) $(ObjectSwitch)Astronom/$(IntermediateDirectory)/MainForm$(ObjectSuffix) $(IncludePath)
Astronom/$(IntermediateDirectory)/MainForm$(DependSuffix): Astronom/MainForm.cpp
	@test -d Astronom/Debug || mkdir -p Astronom/Debug
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MTAstronom/$(IntermediateDirectory)/MainForm$(ObjectSuffix) -MFAstronom/$(IntermediateDirectory)/MainForm$(DependSuffix) -MM "/home/willow/prj/amax-hg/Astronomer/Astronom/MainForm.cpp"

Astronom/$(IntermediateDirectory)/RectangleView$(ObjectSuffix): Astronom/RectangleView.cpp Astronom/$(IntermediateDirectory)/RectangleView$(DependSuffix)
	@test -d Astronom/Debug || mkdir -p Astronom/Debug
	$(CompilerName) $(SourceSwitch) "/home/willow/prj/amax-hg/Astronomer/Astronom/RectangleView.cpp" $(CmpOptions) $(ObjectSwitch)Astronom/$(IntermediateDirectory)/RectangleView$(ObjectSuffix) $(IncludePath)
Astronom/$(IntermediateDirectory)/RectangleView$(DependSuffix): Astronom/RectangleView.cpp
	@test -d Astronom/Debug || mkdir -p Astronom/Debug
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MTAstronom/$(IntermediateDirectory)/RectangleView$(ObjectSuffix) -MFAstronom/$(IntermediateDirectory)/RectangleView$(DependSuffix) -MM "/home/willow/prj/amax-hg/Astronomer/Astronom/RectangleView.cpp"


-include Astronom/$(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) Astronom/$(IntermediateDirectory)/DraggableView$(ObjectSuffix)
	$(RM) Astronom/$(IntermediateDirectory)/DraggableView$(DependSuffix)
	$(RM) Astronom/$(IntermediateDirectory)/DraggableView$(PreprocessSuffix)
	$(RM) Astronom/$(IntermediateDirectory)/GlyphManager$(ObjectSuffix)
	$(RM) Astronom/$(IntermediateDirectory)/GlyphManager$(DependSuffix)
	$(RM) Astronom/$(IntermediateDirectory)/GlyphManager$(PreprocessSuffix)
	$(RM) Astronom/$(IntermediateDirectory)/main$(ObjectSuffix)
	$(RM) Astronom/$(IntermediateDirectory)/main$(DependSuffix)
	$(RM) Astronom/$(IntermediateDirectory)/main$(PreprocessSuffix)
	$(RM) Astronom/$(IntermediateDirectory)/WheelView$(ObjectSuffix)
	$(RM) Astronom/$(IntermediateDirectory)/WheelView$(DependSuffix)
	$(RM) Astronom/$(IntermediateDirectory)/WheelView$(PreprocessSuffix)
	$(RM) Astronom/$(IntermediateDirectory)/Astronom$(ObjectSuffix)
	$(RM) Astronom/$(IntermediateDirectory)/Astronom$(DependSuffix)
	$(RM) Astronom/$(IntermediateDirectory)/Astronom$(PreprocessSuffix)
	$(RM) Astronom/$(IntermediateDirectory)/MainForm$(ObjectSuffix)
	$(RM) Astronom/$(IntermediateDirectory)/MainForm$(DependSuffix)
	$(RM) Astronom/$(IntermediateDirectory)/MainForm$(PreprocessSuffix)
	$(RM) Astronom/$(IntermediateDirectory)/RectangleView$(ObjectSuffix)
	$(RM) Astronom/$(IntermediateDirectory)/RectangleView$(DependSuffix)
	$(RM) Astronom/$(IntermediateDirectory)/RectangleView$(PreprocessSuffix)
	$(RM) $(OutputFile)


