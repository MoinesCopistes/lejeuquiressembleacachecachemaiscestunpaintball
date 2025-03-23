def add_border(map_data):
    lines = map_data.strip().split('\n')
    width = max(len(line) for line in lines)
    

    lines = [line.ljust(width) for line in lines]
    

    bordered_map = ['5' * (width + 4)]
    bordered_map.append('5' + 'I' * (width + 2) + '5')
    

    for line in lines:
        bordered_map.append('5' + 'I' + line + 'I' + '5')

    bordered_map.append('5' + 'I' * (width + 2) + '5')
    bordered_map.append('5' * (width + 4))
    
    return bordered_map

def determine_texture(bordered, y, x):
    current = bordered[y][x]
    if current == '5':
        return '5'
    if current == ' ':
        return ' ' 

    north = bordered[y-1][x]
    south = bordered[y+1][x]
    west = bordered[y][x-1]
    east = bordered[y][x+1]
    
    north_west = bordered[y-1][x-1]
    north_east = bordered[y-1][x+1]
    south_west = bordered[y+1][x-1]
    south_east = bordered[y+1][x+1]

    if north != ' ' and west != ' ' and north_west != ' ' and south == ' ' and east == ' ' and south_east == ' ':
        return 'a'
    

    elif north != ' ' and east != ' ' and north_east != ' ' and south == ' ' and west == ' ' and south_west == ' ':
        return 'z'

    elif west != ' ' and north == ' ' and south != ' ' and east == ' ' and north_east == ' ' and south_west != ' ':
        return 'q'
    

    elif east != ' ' and north == ' ' and south != ' ' and west == ' ' and north_west == ' ' and south_east != ' ':
        return 's'

    elif south != ' ' and east != ' ' and south_east == ' ' and south_west != ' ' and north_west != ' ' and west != ' ' and north != ' ' and north_east != ' ':
        return '1'

    elif south != ' ' and west != ' ' and south_west == ' ' and south_east != ' ' and north_east != ' ' and east != ' ' and north != ' ' and north_west != ' ':
        return '3'
    

    elif north != ' ' and east != ' ' and north_east == ' ' and north_west != ' ' and south_west != ' ' and west != ' ' and south != ' ' and south_east != ' ':
        return '7'
    

    elif north != ' ' and west != ' ' and north_west == ' ' and north_east != ' ' and south_east != ' ' and east != ' ' and south != ' ' and south_west != ' ':
        return '9'
    

    elif north == ' ' and south != ' ' and east != ' ' and west != ' ' :
        return '8'
    

    elif west == ' ' and east != ' ' and north != ' ' and south != ' ':
        return '6'

    elif east == ' ' and west != ' ' and north != ' ' and south != ' ':
        return '4'
    elif north != ' ' and south != ' ' and east != ' ' and west != ' ' and north_west != ' ' and north_east != ' ' and south_west != ' ' and south_east != ' ':
        return '5'

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
    
    # Function to get distance between two positions
    def distance(pos1, pos2):
        return ((pos1[0] - pos2[0])**2 + (pos1[1] - pos2[1])**2)**0.5
    
    # Function to place special characters in well-spaced positions
    def place_special_chars(positions, char, count, min_dist, all_special_positions):
        import random
        selected = []
        max_attempts = 100
        
        while len(selected) < count and max_attempts > 0 and positions:
            candidate = random.choice(positions)
            
            # Check if candidate is far enough from all previously placed special characters
            is_far_enough = True
            for pos in all_special_positions:
                if distance(candidate, pos) < min_dist:
                    is_far_enough = False
                    break
            
            if is_far_enough:
                selected.append(candidate)
                all_special_positions.append(candidate)
                positions.remove(candidate)
            
            max_attempts -= 1
        
        # If we couldn't find enough well-spaced positions, just pick random ones
        while len(selected) < count and positions:
            pos = random.choice(positions)
            selected.append(pos)
            all_special_positions.append(pos)
            positions.remove(pos)
        
        # Place the special characters
        for y, x in selected:
            row_list = list(textured_map[y])
            row_list[x] = char
            textured_map[y] = ''.join(row_list)
        
        return selected
    
    import random
    if len(floor_positions) >= 24:  # Need enough space for all special characters (4+10+10)
        all_special_positions = []
        
        # Place special characters
        place_special_chars(floor_positions, '0', 4, 8, all_special_positions)
        place_special_chars(floor_positions, 'w', 10, 8, all_special_positions)
        place_special_chars(floor_positions, 'x', 10, 8, all_special_positions)
    
    return '\n'.join(textured_map)