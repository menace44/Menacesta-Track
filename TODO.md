# Variable Track Heights Implementation - TODO

## Phase 1: Core Track Enhancement
- [x] Add height property to Track class
- [x] Add height multiplier property (1.0 = normal, 2.0 = double height, etc.)
- [x] Add methods to get calculated height based on multiplier
- [x] Update TrackLaneComponent to use calculated heights

## Phase 2: UI Controls
- [ ] Add height adjustment controls to TrackControlPanel
- [ ] Create visual height indicators in track headers
- [ ] Add mouse drag support for height adjustment
- [ ] Implement height reset functionality

## Phase 3: Timeline Integration
- [ ] Update TimelineViewport to handle variable track heights
- [ ] Ensure proper scrolling with variable heights
- [ ] Update TimelineGrid to align with variable track positions
- [ ] Update selection and drag operations for variable heights

## Phase 4: Persistence
- [ ] Save track heights in project file
- [ ] Load track heights from project file
- [ ] Add default height settings in preferences

## Phase 5: Polish
- [ ] Add smooth animations for height changes
- [ ] Add visual feedback during height adjustment
- [ ] Add keyboard shortcuts for height adjustment
- [ ] Add context menu for height options

## Phase 6: Testing
- [ ] Test with different track types
- [ ] Test height persistence across sessions
- [ ] Test with large numbers of tracks
- [ ] Test performance with extreme height variations
