# ============================================================================ #
#    When importing INP file into Abaqus, cells are considered as sets in the  #
#    assembly. This script copies the sets from the assembly to the 'Part'     #
#    section.                                                                  #
# ============================================================================ #

# Get the current model name and its part name
vps = session.viewports.values()[0]
mdbName=vps.displayedObject.modelName
pk=mdb.models[mdbName].parts.keys()
partName=pk[0]

# Set the new part visible
p1 = mdb.models[mdbName].parts[partName]
session.viewports['Viewport: 1'].setValues(displayedObject=p1)

# Fecth all sets in the assembly
a=mdb.models[mdbName].rootAssembly
sets=a.sets
sets_list=sets.keys()
p = mdb.models[mdbName].parts[partName]

for cellID in sets_list:
  set_i=sets[cellID]
	if cellID.startswith('CELL'):
		IDs=[j.label for j in set_i.elements]
		p.SetFromElementLabels(elementLabels=IDs,name=cellID)
