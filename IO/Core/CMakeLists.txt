SET(Module_SRCS
  vtkAbstractParticleWriter.cxx
  vtkArrayReader.cxx
  vtkArrayWriter.cxx
  vtkASCIITextCodec.cxx
  vtkBase64InputStream.cxx
  vtkBase64OutputStream.cxx
  vtkBase64Utilities.cxx
  vtkDataCompressor.cxx
  vtkDataObjectReader.cxx
  vtkDataObjectWriter.cxx
  vtkDataReader.cxx
  vtkDataWriter.cxx
  vtkDelimitedTextWriter.cxx
  vtkGlobFileNames.cxx
  vtkGraphReader.cxx
  vtkGraphWriter.cxx
  vtkInputStream.cxx
  vtkJavaScriptDataWriter.cxx
  vtkOutputStream.cxx
  vtkSortFileNames.cxx
  vtkTextCodec.cxx
  vtkTextCodecFactory.cxx
  vtkUTF16TextCodec.cxx
  vtkUTF8TextCodec.cxx
  vtkWriter.cxx
  vtkZLibDataCompressor.cxx
  vtkTableReader.cxx
  vtkTableWriter.cxx
  vtkArrayDataReader.cxx
  vtkArrayDataWriter.cxx
  )

include_directories(${VTK_SOURCE_DIR}/ThirdParty/utf8/source)

set_source_files_properties(
  vtkAbstractParticleWriter
  vtkDataCompressor
  vtkTextCodec
  vtkWriter
  ABSTRACT
  )

vtk_module_library(vtkIOCore ${Module_SRCS})
