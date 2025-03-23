def add_border(map_data):
    lines = map_data.strip().split('\n')
    width = max(len(line) for line in lines)
    
    # Normalize line lengths
    lines = [line.ljust(width) for line in lines]
    
    # Add a double border: outer '5' border and inner 'I' border
    # Create top double border
    bordered_map = ['5' * (width + 4)]  # Outer top 5 border
    bordered_map.append('5' + 'I' * (width + 2) + '5')  # Inner top I border
    
    # Add side borders to each line
    for line in lines:
        bordered_map.append('5' + 'I' + line + 'I' + '5')
    
    # Create bottom double border
    bordered_map.append('5' + 'I' * (width + 2) + '5')  # Inner bottom I border
    bordered_map.append('5' * (width + 4))  # Outer bottom 5 border
    
    return bordered_map

def determine_texture(bordered, y, x):
    current = bordered[y][x]
    if current == '5':
        return '5'
    if current == ' ':
        return ' '  # Floor tiles remain as spaces
    
    # Get the states of the 8 cardinal directions
    north = bordered[y-1][x]
    south = bordered[y+1][x]
    west = bordered[y][x-1]
    east = bordered[y][x+1]
    
    north_west = bordered[y-1][x-1]
    north_east = bordered[y-1][x+1]
    south_west = bordered[y+1][x-1]
    south_east = bordered[y+1][x+1]
    
    # Inside corner conditions (checking ALL 8 directions)
    if north != ' ' and west != ' ' and north_west != ' ' and south == ' ' and east == ' ' and south_east == ' ':
        return 'a'
    
    # Texture z (Mirror of a - Vertical Corridor Wall)
    # Pattern:
    # II
    # zI
    elif north != ' ' and east != ' ' and north_east != ' ' and south == ' ' and west == ' ' and south_west == ' ':
        return 'z'
        # Texture q (Horizontal Corridor Wall)
    # Pattern:
    # Iq
    # II
    elif west != ' ' and north == ' ' and south != ' ' and east == ' ' and north_east == ' ' and south_west != ' ':
        return 'q'
    
    # Texture s (Mirror of q - Horizontal Corridor Wall)
    # Pattern:
    # sI
    # II
    elif east != ' ' and north == ' ' and south != ' ' and west == ' ' and north_west == ' ' and south_east != ' ':
        return 's'
    # Texture 1 (Top-Left Inside Corner)
    elif south != ' ' and east != ' ' and south_east == ' ' and south_west != ' ' and north_west != ' ' and west != ' ' and north != ' ' and north_east != ' ':
        return '1'
    
    # Texture 3 (Top-Right Inside Corner)
    elif south != ' ' and west != ' ' and south_west == ' ' and south_east != ' ' and north_east != ' ' and east != ' ' and north != ' ' and north_west != ' ':
        return '3'
    
    # Texture 7 (Bottom-Left Inside Corner)
    elif north != ' ' and east != ' ' and north_east == ' ' and north_west != ' ' and south_west != ' ' and west != ' ' and south != ' ' and south_east != ' ':
        return '7'
    
    # Texture 9 (Bottom-Right Inside Corner)
    elif north != ' ' and west != ' ' and north_west == ' ' and north_east != ' ' and south_east != ' ' and east != ' ' and south != ' ' and south_west != ' ':
        return '9'
    
    # Edge conditions (checking ALL 8 directions)
    # Texture 2 (Top Edge)
    elif north == ' ' and south != ' ' and east != ' ' and west != ' ' :
        return '8'
    
    # Texture 4 (Left Edge)
    elif west == ' ' and east != ' ' and north != ' ' and south != ' ':
        return '6'
    
    # Texture 6 (Right Edge)
    elif east == ' ' and west != ' ' and north != ' ' and south != ' ':
        return '4'
    elif north != ' ' and south != ' ' and east != ' ' and west != ' ' and north_west != ' ' and north_east != ' ' and south_west != ' ' and south_east != ' ':
        return '5'
    # Texture 8 (Bottom Edge)
    elif south == ' ' and north != ' ' and east != ' ' and west != ' ':
        return '2'

    

    return '2'

def texture_map(map_data):
    # Add border
    bordered_map = add_border(map_data)
    
    # Convert to texture numbers
    textured_map = []
    for y in range(len(bordered_map)):
        new_row = []
        for x in range(len(bordered_map[y])):
            if bordered_map[y][x] == '5':
                new_row.append('5')  # Keep border as '5'
            else:
                new_row.append(determine_texture(bordered_map, y, x))
        textured_map.append(''.join(new_row))
    
    # Find all floor (empty space) positions
    floor_positions = []
    for y in range(1, len(textured_map)-1):
        for x in range(1, len(textured_map[y])-1):
            # Look for floor spaces (represented by ' ')
            if textured_map[y][x] == ' ':
                floor_positions.append((y, x))
    
    # Add 4 random zeros in well-spaced floor positions
    import random
    if len(floor_positions) >= 4:
        # Function to get distance between two positions
        def distance(pos1, pos2):
            return ((pos1[0] - pos2[0])**2 + (pos1[1] - pos2[1])**2)**0.5
        
        # Try to find well-spaced positions
        selected_positions = []
        max_attempts = 100
        min_distance = 8  # Minimum distance between zeros
        
        while len(selected_positions) < 4 and max_attempts > 0:
            candidate = random.choice(floor_positions)
            
            # Check if candidate is far enough from all previously selected positions
            is_far_enough = True
            for pos in selected_positions:
                if distance(candidate, pos) < min_distance:
                    is_far_enough = False
                    break
            
            if is_far_enough:
                selected_positions.append(candidate)
                floor_positions.remove(candidate)
            
            max_attempts -= 1
        
        # If we couldn't find 4 well-spaced positions, just pick random ones
        while len(selected_positions) < 4 and floor_positions:
            pos = random.choice(floor_positions)
            selected_positions.append(pos)
            floor_positions.remove(pos)
        
        # Replace the characters at selected positions with '0'
        for y, x in selected_positions:
            row_list = list(textured_map[y])
            row_list[x] = '0'
            textured_map[y] = ''.join(row_list)
    
    return '\n'.join(textured_map)

